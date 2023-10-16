#include <iostream>
#include <queue>
#include <Windows.h>
//--------------//
#include <thread>
//#include <process.h>
#include<atomic>

class SafeQueue
{
private:
    std::atomic_bool alock = 0;
    std::queue<int> queue;
public:
    SafeQueue() { }
    ~SafeQueue() { }
    void push(int elem, int ID)
    {
        while (alock.exchange(true))
        {
            Sleep(5);
        }
        queue.push(elem);
        std::cout << 'P' << ID << " -> " << elem << '\n';
        alock.store(false);
        
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        while (alock.exchange(true))
        {
            Sleep(5);
        }
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

        alock.store(false);
        return result;
    }
    bool empty()
    {
        bool isempty;
        while (alock.exchange(true))
        {
            Sleep(5);
        }
        isempty = queue.empty();
        alock.store(false);
        return isempty;
    }
};
SafeQueue elems_queue;

std::atomic_bool flag = 0;
volatile long work_for_producer = 20;
volatile long work_for_consumer = 20;


void task_producer(int ID)
{
    // пока есть работа у потока-производителя
    while (work_for_producer > 0)
    {

        int elem = rand() % 100;
        Sleep(2);
        elems_queue.push(elem + ID, ID);

        flag.store(false);

        _InterlockedExchangeAdd(&work_for_producer, -1);

    }
}
void task_consumer(int ID)
{
    while (work_for_consumer > 0)
    {
        int elem;
        while (flag.exchange(true))
        {
            Sleep(5);
            if (!elems_queue.empty() || !flag.exchange(true))
            {
                flag.store(false);
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
    flag.store(false);
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