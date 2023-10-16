#include <iostream>
#include <string>
#include <fstream>
//-------------
#include <vector>
using std::cin;
using std::fstream;
using std::cout;

const int LINE = 5;
const int COL = 5;
std::vector<std::vector<int>> MATRIX;
// ways
/*
int MAX=0;
int COUNTER;
void task2(int cur_sum, int x, int y)
{
    if (y==LINE-1 && x==COL-1)
    {
        if (cur_sum > MAX)
        {
            MAX = cur_sum;
            COUNTER=1;
        }
        else if(cur_sum == MAX)
        {
            ++COUNTER;
        }
    }
    else
    {
        if (x+1 < COL)
        {
            task2((cur_sum+MATRIX[y][x+1]), x+1, y);
        }
        if (y + 1 < LINE)
        {
            task2((cur_sum+MATRIX[y+1][x]), x, y+1);
        }
    }
}
*/

// zero
/*
int MIN=9999999;
int MAX = -1;
void task2(int cur_sum, int x, int y, const std::string &MINMAX, int zero_counter = 0)
{
    if (x==COL-1 && y == LINE-1)
    {
        zero_counter += (MATRIX[0][0] == 0) ? 1 : 0;
        if (zero_counter==2)
        {
            if (MINMAX=="MAX" && MAX<cur_sum)
                MAX = cur_sum;
            
            if(MINMAX=="MIN"&&MIN>cur_sum)
                MIN = cur_sum;
        }
    }
    else
    {
        if (x+1<COL)
        {
            if (MATRIX[y][x + 1] == 0)
                task2(cur_sum+MATRIX[y][x + 1], x + 1, y, MINMAX, zero_counter+1);
            else
                task2(cur_sum + MATRIX[y][x + 1], x + 1, y, MINMAX, zero_counter);
        }
        if (y+1<LINE)
        {
            if (MATRIX[y + 1][x] == 0)
                task2(cur_sum+MATRIX[y+1][x], x, y+1, MINMAX, zero_counter+1);
            else
                task2(cur_sum+MATRIX[y+1][x], x, y+1, MINMAX, zero_counter);
        }
    }
}
*/

int main()
{
    
    MATRIX.resize(LINE);
    for (size_t i = 0; i < LINE; i++)
    {
        MATRIX[i].resize(COL);
    }
    fstream file("matrix.txt");
    int number = 0;
    for (size_t i = 0; i < LINE; i++)
    {
        for (size_t j = 0; j < COL; j++)
        {
            file >> number;
            MATRIX[i][j] = number;
        }
    }
    for (size_t i = 0; i < LINE; i++)
    {
        for (size_t j = 0; j < COL; j++)
        {
            cout << MATRIX[i][j]<<' ';
        }
        cout << '\n';
    }
    // ways
    /*
    task2(MATRIX[0][0], 0, 0);
    cout << "MAX: " << MAX << " WAYS: " << COUNTER;
    cout << "\nDone.";
    */

    // zero
    /*
    task2(MATRIX[0][0], 0, 0, "MAX");
    task2(MATRIX[0][0], 0, 0, "MIN");
    cout << "\nDone.";
    cout << "MAX: " << MAX << " MIN: " << MIN;
    */

    cin.get();
    return 0;
}