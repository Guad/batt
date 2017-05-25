using System;
using System.IO;

namespace BattProgrammingLanguage
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("USAGE: battc [path to program]");
                return;
            }

            string path = args[0];
            string buffer = File.ReadAllText(path);

            BattCompiler comp = new BattCompiler();

            object[] instructions = comp.Compile(buffer);

            BattInterpreter interp = new BattInterpreter(instructions);

            interp.Run();
        }
    }
}