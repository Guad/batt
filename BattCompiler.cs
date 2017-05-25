using System;
using System.Collections.Generic;

namespace BattProgrammingLanguage
{
    public class BattCompiler
    {
        public object[] Compile(string program)
        {
            program = Preprocess(program);

            List<object> tokens = new List<object>();
            string buffer = "";
            bool inQuote = false;
            int wraps = 0;

            for (int i = 0; i < program.Length; i++)
            {
                // Skip spaces
                if ((program[i] == ' ' || program[i] == '\t') && !inQuote)
                {
                    if (!string.IsNullOrWhiteSpace(buffer))
                    {
                        object token = Tokenizer.ConvertLiteral(buffer, wraps);
                        buffer = "";
                        wraps = 0;
                        tokens.Add(token);
                    }
                    continue;
                }

                buffer += program[i];
                if (program[i] == '"') inQuote = !inQuote;
                if (program[i] == '@') wraps++;
            }

            if (!string.IsNullOrWhiteSpace(buffer))
            {
                object token = Tokenizer.ConvertLiteral(buffer, wraps);
                tokens.Add(token);
            }

            return tokens.ToArray();
        }

        // Strip newlines, comments
        private string Preprocess(string prog)
        {
            string output = "";

            bool inComment = false;

            for (int i = 0; i < prog.Length; i++)
            {
                if (prog[i] == '\r' || prog[i] == '\n')
                {
                    output += " ";
                    inComment = false;
                    continue;
                }

                if (prog[i] == ';')
                    inComment = true;

                if (!inComment)
                    output += prog[i];
            }

            return output;
        }
    }

    public class CompileException : Exception
    {
        private string _message;
        public override string Message => _message;

        public CompileException(string msg)
        {
            _message = msg;
        }

    }
}