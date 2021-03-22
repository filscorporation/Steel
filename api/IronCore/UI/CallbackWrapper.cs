namespace Iron
{
    internal class CallbackWrapper
    {
        private event CallbackAction Delegate;

        public CallbackWrapper(CallbackAction action)
        {
            Delegate += action;
        }

        public void Invoke()
        {
            Delegate?.Invoke();
        }

        public override bool Equals(object obj)
        {
            if (obj is CallbackWrapper cw)
                return Delegate == cw.Delegate;
            
            return false;
        }

        public override int GetHashCode()
        {
            return Delegate?.GetHashCode() ?? 0;
        }
    }
}