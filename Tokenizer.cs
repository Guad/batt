using System.Linq;

namespace BattProgrammingLanguage
{
    public class Tokenizer
    {
        public static object ConvertLiteral(string obj, int wraps)
        {
            // Declares a function
            if (obj == "proc")
                return new FuncDef();

            // Declares a function end
            if (obj == "end")
                return new FuncDefEnd();

            // Wrap puts a functioncall on the stack, instead of executing it.
            if (wraps > 0)
                return new Wrap(ConvertLiteral(obj.Substring(1), wraps - 1));

            obj = obj.Trim();

            // string?
            if (obj.StartsWith("\"") && obj.EndsWith("\""))
                return obj.Trim('"');

            int dots = 0;
            // float?
            // If it's a decimal point, it'll fall through and increment dots
            if (obj.All(c => char.IsDigit(c) || ++dots < 0 || c == '.') && dots <= 1)
            {
                return float.Parse(obj);
            }

            // Otherwise, it's a function call.
            return new FuncCall(obj);
        }
    }

    public class FuncCall
    {
        public FuncCall(string name)
        {
            Name = name;
        }

        public string Name { get; set; }
    }

    public class FuncDef { }
    public class FuncDefEnd { }
}