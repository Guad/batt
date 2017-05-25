using System;
using System.Reflection;

namespace BattProgrammingLanguage
{
    public partial class BattInterpreter
    {
		[FunctionAlias("+")]
        public void sum()
		{
		    object right = Memory.Pop();
		    object left = Memory.Pop();
		    if (right is string || left is string)
		    {
		        Memory.Push(left.ToString() + right.ToString());
		    }
			else Memory.Push(Convert.ToSingle(left) + Convert.ToSingle(right));
        }

        [FunctionAlias("-")]
        public void sub()
        {
            float right = Convert.ToSingle(Memory.Pop());
            float left = Convert.ToSingle(Memory.Pop());
            Memory.Push(left - right);
        }

        [FunctionAlias("*")]
        public void mul()
        {
            float right = Convert.ToSingle(Memory.Pop());
            float left = Convert.ToSingle(Memory.Pop());
            Memory.Push(left * right);
        }

        [FunctionAlias("/")]
        public void div()
        {
            float right = Convert.ToSingle(Memory.Pop());
            float left = Convert.ToSingle(Memory.Pop());
            Memory.Push(left / right);
        }

        [FunctionAlias("%")]
        public void mod()
        {
            float right = Convert.ToSingle(Memory.Pop());
            float left = Convert.ToSingle(Memory.Pop());
            Memory.Push((float) ((int)left % (int)right));
        }

        public void apply()
        {
            ExecuteCmd(Memory.Pop());
        }

        public void print()
        {
            Console.Write(Memory.Pop());
        }

        public void println()
        {
            Console.WriteLine(Memory.Pop());
        }

        public void getln()
        {
            Memory.Push(Console.ReadLine());
        }

        public void getch()
        {
            Memory.Push(((char)Console.Read()).ToString());
        }

        public void dup()
        {
            Memory.Push(Memory.Peek());
        }

        public void pop()
        {
            Memory.Pop();
        }

        public void swap()
        {
            object right = Memory.Pop();
            object left = Memory.Pop();

            Memory.Push(right);
            Memory.Push(left);
        }

        public void dip()
        {
            object tmp = Memory.Pop();
            object quot = Memory.Pop();

            ExecuteCmd(quot);
            Memory.Push(tmp);
        }

        public void over()
        {
            object tmp = Memory.Pop();
            object target = Memory.Peek();

            Memory.Push(tmp);
            Memory.Push(target);
        }

        [FunctionAlias("ifelse")]
        public void condelse()
        {
            object second = Memory.Pop();
            object first = Memory.Pop();
            float val = Convert.ToSingle(Memory.Pop());

            if (val != 0)
                ExecuteCmd(first);
            else
                ExecuteCmd(second);
        }

        [FunctionAlias("if")]
        public void cond()
        {
            object left = Memory.Pop();
            float val = Convert.ToSingle(Memory.Pop());

            if (val != 0)
                ExecuteCmd(left);
        }

        [FunctionAlias("=")]
        public void equals()
        {
            object right = Memory.Pop();
            object left = Memory.Pop();

            if (right.Equals(left))
                Memory.Push(1f);
			else Memory.Push(0f);
        }

		[FunctionAlias(">")]
        public void gt()
        {
            float right = Convert.ToSingle(Memory.Pop());
            float left = Convert.ToSingle(Memory.Pop());

			Memory.Push(left > right ? 1f : 0f);
        }

        [FunctionAlias("<")]
        public void lt()
        {
            float right = Convert.ToSingle(Memory.Pop());
            float left = Convert.ToSingle(Memory.Pop());

            Memory.Push(left < right ? 1f : 0f);
        }

        [FunctionAlias("!")]
        public void not()
        {
            object val = Memory.Pop();

            if (val is float)
                Memory.Push((float) val == 0f ? 1f : 0f);
            else if (val is string)
			    Memory.Push(string.IsNullOrWhiteSpace((string) val) ? 1f : 0f);
        }

        public void pass()
        {
            
        }
        
        public void dump()
        {
            var arr = Memory.ToArray();

            foreach (object o in arr)
            {
                Console.Write(o.ToString() + ", ");
            }

            Console.WriteLine();
        }
        
    }
}