using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace BattProgrammingLanguage
{
    public partial class BattInterpreter
    {
        // Main stack
        public readonly Stack<object> Memory;

        // All of the instructions. Will not be resized once initialized.
        private object[] _instructions;

        // Are we inside of a function definition?
        // Not to be confused with being inside of a function.
        private bool _insideFunctionDef;

        // Current program counter
        private int _instructionCounter;

        // Cached BattInterpreter type definition.
        private readonly Type _myself;
        private readonly Dictionary<string, string> _funcsAliasMap;
        private readonly Dictionary<string, int>    _functionDefinitions;

        // Callstack is separate from the main stack to allow passing
        // data between call levels.
        private readonly Stack<int> _callStack;

        [BattInvisible]
        public BattInterpreter(object [] tokens)
        {
            Memory = new Stack<object>();

            _instructionCounter = 0;
            _instructions = tokens;

            _myself = typeof(BattInterpreter);
            _funcsAliasMap = new Dictionary<string, string>();
            _functionDefinitions = new Dictionary<string, int>();
            _callStack = new Stack<int>();

            // Cache all aliases
            foreach (var methodInfo in _myself.GetMethods().Where(m => m.GetCustomAttribute<FunctionAlias>() != null))
            {
                _funcsAliasMap.Add(methodInfo.GetCustomAttribute<FunctionAlias>().Name, methodInfo.Name);
            }
        }

        // Execute one instruction
        [BattInvisible]
        public void Step()
        {
            object token = _instructions[_instructionCounter++];

            ExecuteCmd(token);
        }

        // Run to end of program.
        [BattInvisible]
        public void Run()
        {
            for (; _instructionCounter < _instructions.Length;)
            {
                Step();
            }
        }

        public void Abort()
        {
            _instructionCounter = Int32.MaxValue;
        }

        // Execute instruction/literal
        private void ExecuteCmd(object token)
        {
            if (_insideFunctionDef)
            {
                // While we are reading a function definition, ignore all instructions inside.
                if (token is FuncDefEnd)
                    _insideFunctionDef = false;
            }
            else if (token is Wrap)
            {
                object val = ((Wrap) token).Value;
                Memory.Push(val);
            }
            else if (token is FuncCall)
            {
                string funcName = ((FuncCall) token).Name;
                try
                {
                    Call(funcName);
                }
                catch (TargetInvocationException ex)
                {
                    Console.WriteLine($"ERROR @ {funcName}:{_instructionCounter - 1}: {ex?.InnerException?.Message}");
                    Abort();
                }
                catch (MissingMemberException ex)
                {
                    Console.WriteLine($"ERROR @ {funcName}:{_instructionCounter - 1}: {ex?.Message}");
                    Abort();
                }
            }
            else if (token is FuncDef)
            {
                // We are entering a function definition
                _insideFunctionDef = true;
                string name = ((FuncCall)_instructions[_instructionCounter++]).Name;
                _functionDefinitions.Add(name, _instructionCounter);
            }
            else if (token is FuncDefEnd)
            {
                int returnCall = _callStack.Pop();
                _instructionCounter = returnCall;
            }
            else
            {
                Memory.Push(token); // It's a literal - push it onto the stack
            }
        }

        // Call a function
        [BattInvisible]
        private void Call(string methodName)
        {
            // If it's user defined function, push the PC onto the stack and jump.
            if (_functionDefinitions.ContainsKey(methodName))
            {
                _callStack.Push(_instructionCounter);
                _instructionCounter = _functionDefinitions[methodName];
            }
            else
            {
                if (_funcsAliasMap.ContainsKey(methodName))
                    methodName = _funcsAliasMap[methodName];

                var method = _myself.GetMethod(methodName, BindingFlags.Instance | BindingFlags.Public);
                if (method != null && method.GetCustomAttribute<BattInvisibleAttribute>() == null)
                {
                    method.Invoke(this, null);
                }
                else
                {
                    throw new MissingMethodException($"Function {methodName} has not been found.");
                }
            }
        }
    }

    // a BATT program cannot call methods with this attribute
    [AttributeUsage(AttributeTargets.Constructor | AttributeTargets.Field | AttributeTargets.Method, AllowMultiple = false)]
    public class BattInvisibleAttribute : Attribute { }

    // Adds an alias for a method.
    [AttributeUsage(AttributeTargets.Constructor | AttributeTargets.Field | AttributeTargets.Method, AllowMultiple = false)]
    public class FunctionAlias : Attribute
    {
        public readonly string Name;

        public FunctionAlias(string name)
        {
            Name = name;
        }
    }
}