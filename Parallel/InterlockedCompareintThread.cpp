#include <iostream>
#include <queue>
#include <Windows.h>
//--------------//
#include <thread>
//#include <process.h>
//#include <amp.h>

class SafeQueue
{
private:
    volatile long alock;
    std::queue<int> queue;
public:
    SafeQueue() { alock = 0; }
    ~SafeQueue() { }
    void push(int elem, int ID)
    {
        while (_InterlockedCompareExchange(&alock, 1, 0) == 1)
            Sleep(5);
        queue.push(elem);
        std::cout << 'P' << ID << " -> " << elem << '\n';
        _InterlockedCompareExchange(&alock, 0, 1);
        
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        while (_InterlockedCompareExchange(&alock, 1, 0) == 1)
            Sleep(5);
        if (!queue.empty())
        {
            result = true;
            elem = queue.front();
            queue.pop();
            std::cout << 'C' << ID << " <- " << elem << '\n';
        }
        else
        {
            //std::cout << 'C' << ID << " sleep\n";
        }

        _InterlockedCompareExchange(&alock, 0, 1);
        return result;
    }
    bool empty()
    {
        bool isempty;
        while (_InterlockedCompareExchange(&alock, 1, 0) == 1)
            Sleep(5);
        isempty = queue.empty();
        _InterlockedCompareExchange(&alock, 0, 1);
        return isempty;
    }
};
SafeQueue elems_queue;

volatile long flag=0;
volatile long work_for_producer = 20;
volatile long work_for_consumer = 20;


void task_producer(int ID)
{
    // ���� ���� ������ � ������-�������������
    while (work_for_producer > 0)
    {

        int elem = rand() % 100;
        Sleep(2);
        elems_queue.push(elem + ID, ID);

        _InterlockedExchange(&flag, 0);

        _InterlockedExchangeAdd(&work_for_producer, -1);

    }
}
void task_consumer(int ID)
{
    while (work_for_consumer > 0)
    {
        int elem;
        
        while (_InterlockedCompareExchange(&flag, 0,1)==1)
        {
            Sleep(5);
            if (!elems_queue.empty() || !flag)
            {
                _InterlockedCompareExchange(&flag, 1, 0);
            }
        }
        

        if (elems_queue.pop(elem, ID))
        {
            Sleep(5);
        }
        else
        {
            _InterlockedExchangeAdd(&work_for_consumer, 1);
        }

        _InterlockedExchangeAdd(&work_for_consumer, -1);
    }
    _InterlockedCompareExchange(&flag, 1, 0);
}

int main()
{
    
    srand(GetTickCount());
    
    

    std::thread task4[5];
    for (int i = 0; i < 5; ++i)
    {
        if (i < 2)
        {
            task4[i] = std::thread(&task_producer, i);
        }
        else
        {
            task4[i] = std::thread(&task_consumer, i);
        }
    }

    for (size_t i = 0; i < 5; ++i)
        task4[i].join();
    std::cout << "Done.";
    /*while (!add_one.empty())
    {
        std::cout << add_one.front() << '\n';
        add_one.pop();
    }*/
    std::cin.get();
    return 0;
}