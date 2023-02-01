using System;

namespace Events
{
    internal class Account
    {
        public delegate void AccountHandler(string message);

        public event AccountHandler? Notify;              // 1. create event

        public Account(int sum) => Sum = sum;

        public int Sum { get; private set; }

        public void Put(int sum)
        {
            Sum += sum;
            Notify?.Invoke($"На счет поступило: {sum}");   // 2. event call
        }

        public void Take(int sum)
        {
            if (Sum >= sum)
            {
                Sum -= sum;
                Notify?.Invoke($"Со счета снято: {sum}");   // 2. event call
            }
            else
            {
                Notify?.Invoke($"Недостаточно денег на счете. Текущий баланс: {Sum}"); ;
            }
        }
    }

    //--------------------------------------------------------------------------------//
    public delegate void myEventHandler(string value);

    internal class EventPublisher
    {
        private string theVal;

        // declare the event handler
        public event myEventHandler valueChanged;

        public event EventHandler<ObjChangeEventArgs> objChanged;

        public string Val
        {
            set
            {
                this.theVal = value;
                // when the value changes, fire the event
                this.valueChanged(theVal);
                this.objChanged(this, new ObjChangeEventArgs() { propChanged = "Val" });
            }
        }
    }

    internal class ObjChangeEventArgs : EventArgs
    {
        public string propChanged;
    }

    //----------------------------------Example with Bank------------------------------------------//
    public delegate void BalanceEventHandler(decimal theValue);

    internal class PiggyBank
    {
        private decimal m_bankBalance;

        public event BalanceEventHandler balanceChanged;

        public decimal theBalance   // demical support huge numbers
        {
            set
            {
                m_bankBalance = value;
                balanceChanged(value);
            }
            get
            {
                return m_bankBalance;
            }
        }
    }

    internal class BalanceLogger
    {
        public void balanceLog(decimal amount)
        {
            Console.WriteLine("The balance amount is {0}", amount);
        }
    }

    internal class BalanceWatcher
    {
        public void balanceWatch(decimal amount)
        {
            if (amount > 50)
                Console.WriteLine("You reached your savings goal! You have {0}", amount);
        }
    }

    //-------------------------------------------------------------------------------//
}