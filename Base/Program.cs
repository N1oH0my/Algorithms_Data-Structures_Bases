using System;
using System.Collections.Generic;   // list~

using System.IO; // files

namespace Base
{
    class Program
    {
        enum TrafficLights { Green, Red, Yellow };
        //----------------------------Start Main--------------------------------//
        static void Main()
        {

            //-----------------------------Convert------input/output--------------------//
            
            double dNumber = 23.15;

            try
            {
                // Returns 23
                int iNumber = System.Convert.ToInt32(dNumber);
            }
            catch (System.OverflowException)
            {
                System.Console.WriteLine(
                            "Overflow in double to int conversion.");
            }
            // Returns True
            bool bNumber = System.Convert.ToBoolean(dNumber);

            // Returns "23.15"
            string strNumber = System.Convert.ToString(dNumber);

            try
            {
                // Returns '2'
                char chrNumber = System.Convert.ToChar(strNumber[0]);
            }
            catch (System.ArgumentNullException)
            {
                System.Console.WriteLine("String is null");
            }
            catch (System.FormatException)
            {
                System.Console.WriteLine("String length is greater than 1.");
            }

            // System.Console.ReadLine() returns a string and it
            // must be converted.
            int newInteger = 0;
            try
            {
                System.Console.WriteLine("Enter an integer:");
                newInteger = System.Convert.ToInt32(
                                    System.Console.ReadLine());
            }
            catch (System.ArgumentNullException)
            {
                System.Console.WriteLine("String is null.");
            }
            catch (System.FormatException)
            {
                System.Console.WriteLine("String does not consist of an " +
                                "optional sign followed by a series of digits.");
            }
            catch (System.OverflowException)
            {
                System.Console.WriteLine(
                "Overflow in string to int conversion.");
            }

            System.Console.WriteLine("Your integer as a double is {0}",
                                     System.Convert.ToDouble(newInteger));
            //----------------------------Arrays----------------------------//
            int[,] someNums = { 
                { 2, 3 }, 
                { 5, 6 }, 
                { 4, 6 } 
            };
            Console.WriteLine(someNums.Length);
            Console.WriteLine(someNums.Rank);
            int[,] dynamicArr = new int[4, 4];
            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    dynamicArr[i, j] = 1;
                }
            }
            for (int k = 0; k < 3; k++)
            {
                for (int j = 0; j < 2; j++)
                {
                    Console.Write(someNums[k, j] + " ");
                }
                Console.WriteLine();
            }

            //-------------------------Enum-----Switch---------------------------//
            TrafficLights x = TrafficLights.Red;
            switch (x)
            {
                case TrafficLights.Green:
                    Console.WriteLine("Go!");
                    break;
                case TrafficLights.Red:
                    Console.WriteLine("Stop!");
                    break;
                case TrafficLights.Yellow:
                    Console.WriteLine("Caution!");
                    break;
                default:
                    Console.WriteLine("The default case");
                    break;
            }
            //-------------------------Func--------------------------------//
            int a = 3;
            Sqr(a);
            // ref == &, * already here
            //------------------------------------------------------------//
            Person person = new Person("Boba");
            person.Hi();
            string name = person.Name;
            person.Name = name;
            //------------------------------Abstract/ override----------------------------------//
            Shape cric = new Circle();
            cric.Draw();
            IShape c = new Cir();
            c.Draw();
            //------------------------------Files------------------------------------//
            String line;
            try
            {
                //Pass the file path and file name to the StreamReader constructor
                StreamReader sr = new StreamReader(/*"C:\\Sample.txt"*/ "D:\\CshRepos\\Base\\test.txt");
                //Read the first line of text
                line = sr.ReadLine();
                //Continue to read until you reach end of file
                while (line != null)
                {
                    //write the line to console window
                    Console.WriteLine(line);
                    //Read the next line
                    line = sr.ReadLine();
                }
                //close the file
                sr.Close();
                Console.ReadLine();                     // like cin.get()
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception: " + e.Message);
            }
            finally
            {
                Console.WriteLine("Finally block. end of reading file");
            }
            //---
            try
            {
                //Pass the filepath and filename to the StreamWriter Constructor
                StreamWriter sw = new StreamWriter("C:\\Test.txt");
                //Write a line of text
                sw.WriteLine("Hello World!!");
                //Write a second line of text
                sw.WriteLine("From the StreamWriter class");
                //Close the file
                sw.Close();
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception: " + e.Message);
            }
            finally
            {
                Console.WriteLine("Finally block. end of writing in file");
            }
            //------------------------------T------------------------------------//
            int u = 4, b = 9;
            Swap<int>(ref u, ref b);
            Console.WriteLine(u + " " + b);
            //---
            Stack<int> intStack = new Stack<int>();
            intStack.Push(3);
            intStack.Push(6);
            intStack.Push(7);
            Console.WriteLine(intStack.Get());
            //---
            List<int> li = new List<int>();
            li.Add(59);
            li.Add(72);
            li.Add(95);
            li.Add(5);
            li.Add(9);
            li.RemoveAt(1); // remove 72

            Console.Write("\nList: ");
            for (int i = 0; i < li.Count; i++)
                Console.Write(li[i] + " "); // 59  95  5  9
            li.Sort();
            Console.Write("\nSorted: ");
            for (int i = 0; i < li.Count; i++)
                Console.Write(li[i] + " "); // 5  9  59  95
            //------------------------------------------------------------------//
            //------------------------------------------------------------------//
            Console.ReadLine();
        }
        //--------------------------End Main----------------------------------//
        //--------------doesnt matter where----------------------------//
        class Person
        {
            private string name;
            public string Name
            {
                get { return name; }
                set { name = value; }
            }
            public Person(string _name)
            {
                name = _name;
            }
            public void Hi()
            {
                Console.WriteLine("\n Hi!\n");
            }
        }
        //--------------------------Template----------------------------------//
        static void Swap<T>(ref T a, ref T b)
        {
            T temp = a;
            a = b;
            b = temp;
        }
        //---
        class Stack<T>
        {
            int index = 0;
            T[] innerArray = new T[100];
            public void Push(T item)
            {
                innerArray[index++] = item;
            }
            public T Pop()
            {
                return innerArray[--index];
            }
            public T Get() { return innerArray[index-1]; }
        }
        //------------------------------------------------------------//
        static void Sqr(int x)
        {
            x = x * x;
        }
        //--------------------------Abstract class----------------------------------//
        abstract class Shape
        {
            protected int a;
            public int A
            {
                get { return a; }
                set { a = value; }
            }
            public Shape() { }
            public abstract void Draw();
        }
        class Circle : Shape
        {
            public Circle() { }
            public override void Draw()
            {
                Console.WriteLine("Circle Draw");
            }
        }
        //-----------------------------public and abstract by default-------------------------------//
        public interface IShape
        {
            void Draw();
        }
        class Cir : IShape
        {
            public void Draw()
            {
                Console.WriteLine("Circle Draw by interface");
            }
        }
        //------------------------------------------------------------//
    }
}
