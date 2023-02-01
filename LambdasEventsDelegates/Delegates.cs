using System;

namespace Delegates
{
    public delegate int AddDelegate(int num1, int num2);

    public delegate void showDelegate(string s);

    internal class Func
    {
        //----------------------------func for delegate--------------------------------//
        // params similar to delegate
        public static int Add(int num1, int num2)
        {
            Console.WriteLine("I am called by Delegate");
            int sumation;
            sumation = num1 + num2;
            return sumation;
        }

        //---other
        public static void Display(string title)
        {
            Console.WriteLine(title);
        }

        public static void Show(string title)
        {
            Console.WriteLine(title);
        }

        //------------------------------------------------------------//
    }
}