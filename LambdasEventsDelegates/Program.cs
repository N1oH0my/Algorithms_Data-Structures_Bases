using Delegates;

using Events;

using System;

namespace Base
{
    internal class Program
    {
        private delegate void voidOperation(int x, int y);

        private delegate int Operation(int x, int y);

        private delegate bool IsMore(int x, int y);

        //----------------------------Start Main--------------------------------//
        private static void Main()
        {
            //----------------------------Delegate--------------------------------//
            // create
            AddDelegate funct1 = new AddDelegate(Func.Add);
            AddDelegate funct2; funct2 = Func.Add;
            // Вызов делегата
            int k1 = funct1(7, 2);
            int k2 = funct1(7, 1);
            Console.WriteLine("Sumation = {0}, {1}", k1, k2);

            //---add more func in 1 deleg
            showDelegate s = Func.Display;
            s += Func.Show;
            s("Hello");
            s("Scott");

            //----------------------------Events--------------------------------//
            Account account = new Account(100);
            account.Notify += DisplayMessage;   // Добавляем обработчик для события Notify
            account.Put(20);    // add 20
            //Console.WriteLine($"Сумма на счете: {account.Sum}");
            account.Take(70);   // take from ac 70
            //Console.WriteLine($"Сумма на счете: {account.Sum}");
            account.Take(180);  // try to take 180
            //Console.WriteLine($"Сумма на счете: {account.Sum}");

            void DisplayMessage(string message) => Console.WriteLine(message);

            //-----------Bank events
            PiggyBank pb = new PiggyBank();
            BalanceLogger bl = new BalanceLogger();
            BalanceWatcher bw = new BalanceWatcher();   // after 50 and more

            pb.balanceChanged += bl.balanceLog;
            pb.balanceChanged += bw.balanceWatch;

            string theStr;
            do
            {
                Console.WriteLine("How much to deposit?");

                theStr = Console.ReadLine();
                if (!theStr.Equals("exit"))
                {
                    decimal newVal = decimal.Parse(theStr);

                    pb.theBalance += newVal;
                }
            } while (!theStr.Equals("0"));

            //----------Sys events
            // create the test class
            EventPublisher obj = new EventPublisher();
            // Connect multiple event handlers
            obj.valueChanged += new myEventHandler(changeListener1);
            obj.valueChanged += new myEventHandler(changeListener2);

            // Use an anonymous delegate as the event handler
            obj.valueChanged += delegate (string s)
            {
                Console.WriteLine("This came from the anonymous handler!");
            };

            obj.objChanged += delegate (object sender, ObjChangeEventArgs e)
            {
                Console.WriteLine("{0} had the '{1}' property changed", sender.GetType(), e.propChanged);
            };

            string str;
            do
            {
                Console.WriteLine("Enter a value: ");
                str = Console.ReadLine();
                if (!str.Equals("0"))
                {
                    obj.Val = str;
                }
            } while (!str.Equals("0"));
            Console.WriteLine("bye!");
            //----------------------------Lyambda--------------------------------//
            //---delegates
            voidOperation sum = (x, y) => Console.WriteLine($"{x} + {y} = {x + y}");
            sum(1, 2);       // 1 + 2 = 3

            Operation multiply = (x, y) => x * y;
            int multiplyResult = multiply(4, 5);        // 20
            Console.WriteLine(multiplyResult);          // 20

            //---func
            Func<int, string, int> subtract = (int x, string s) =>
            {
                if (x > 0 && s == "+++")
                {
                    return x;
                }
                else
                {
                    return 0;
                }
            };
            int result1 = subtract(10, "+++");  // 4
            Console.WriteLine(result1);

            //---add del
            Action hello = () => Console.WriteLine("HELLO");

            Action message = () => Console.Write("Hello ");
            message += () => Console.WriteLine("World"); // noname lyambda
            message += hello;   // add func
            message += Print;   // add method

            message();
            Console.WriteLine("--------------"); // для разделения вывода

            message -= Print;   // del method
            message -= hello;   // ~
            message?.Invoke();  // if message have no func/method
            void Print() => Console.WriteLine("Welcome");

            //---like param
            funcWithLyambda1(subtract);
            IsMore cmp = (int x, int y) => { return x > y; };
            Console.WriteLine(funcWithLyambda2(10, 2, cmp));
            //------------------------------------------------------------//
            Console.Read();
        }
        //--------------------------End Main----------------------------------//
        private static void funcWithLyambda1(Func<int, string, int> subsubtract)
        {
            int x = -2;
            string s = "+++";
            int res = subsubtract(x, s);
            Console.WriteLine(res);
        }

        private static int funcWithLyambda2(int x, int y, IsMore cmp)
        {
            if (cmp(x, y))
                return x + y;
            else
                return x - y;
        }
        //--------------------------------------------------------------------//
        private static void changeListener1(string value)
        {
            Console.WriteLine("The value changed to {0}", value);
        }
        private static void changeListener2(string value)
        {
            Console.WriteLine("I also listen to the event, and got {0}", value);
        }
        //------------------------------------------------------------//
    }
}