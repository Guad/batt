namespace BattProgrammingLanguage
{
    public class Wrap
    {
        public object Value;

        public Wrap(object v)
        {
            Value = v;
        }

        public override string ToString()
        {
            return $"[{Value}]";
        }
    }
}