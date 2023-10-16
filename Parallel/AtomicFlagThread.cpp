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
    std::atomic_flag alock = ATOMIC_FLAG_INIT;
    std::queue<int> queue;
public:
    SafeQueue() { }
    ~SafeQueue() { }
    void push(int elem, int ID)
    {
        while (alock.test_and_set())
        {
            Sleep(5);
        }
        queue.push(elem);
        std::cout << 'P' << ID << " -> " << elem << '\n';
        alock.clear(std::memory_order_release);
        
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        while (alock.test_and_set())
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
        
        alock.clear(std::memory_order_release);
        return result;
    }
    bool empty()
    {
        bool isempty;
        while (alock.test_and_set())
        {
            Sleep(5);
        }
        isempty = queue.empty();
        alock.clear(std::memory_order_release);
        return isempty;
    }
};
SafeQueue elems_queue;

std::atomic_flag flag = ATOMIC_FLAG_INIT;
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

        flag.clear(std::memory_order_release);

        _InterlockedExchangeAdd(&work_for_producer, -1);

    }
}
void task_consumer(int ID)
{
    while (work_for_consumer > 0)
    {
        int elem;
        while (flag.test_and_set())
        {
            Sleep(5);
            if (!elems_queue.empty() || !flag.test_and_set())
            {
                flag.clear(std::memory_order_release);
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
    flag.clear(std::memory_order_release);
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