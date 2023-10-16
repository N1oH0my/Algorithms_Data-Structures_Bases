#include <iostream>
#include <queue>
#include <Windows.h>
#include<ctime>
//--------------//
#include <thread>
#include <future>
#include <omp.h>
class SafeQueue
{
private:
    omp_lock_t hELock;
    std::queue<int> queue;
public:
    SafeQueue() { omp_init_lock(&hELock); }
    ~SafeQueue() { omp_destroy_lock(&hELock); }
    void push(int elem, int ID)
    {
        omp_set_lock(&hELock);
        queue.push(elem);
        std::cout << 'P' << ID << " -> " << elem << '\n';
        ReleaseMutex(hELock);
        omp_unset_lock(&hELock);
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        omp_set_lock(&hELock);
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
        omp_unset_lock(&hELock);
        return result;
    }
    bool empty()
    {
        bool isempty;
        omp_set_lock(&hELock);
        isempty = queue.empty();
        
        omp_unset_lock(&hELock);
        return isempty;
    }
};
SafeQueue elems_queue;

volatile long flag=true;
volatile long work_for_producer = 100;
volatile long work_for_consumer = 100;


void task_producer(int ID)
{
    // пока есть работа у потока-производителя
    while (_InterlockedExchangeAdd(&work_for_producer, -1) > 0)
    {

        int elem = rand() % 100;
        Sleep(2);
        elems_queue.push(elem + ID, ID);

        _InterlockedExchange(&flag, false);
    }
}
void task_consumer(int ID)
{
    while (_InterlockedExchangeAdd(&work_for_consumer, -1) > 0)
    {
        int elem;
        while (_InterlockedExchange(&flag, true))
        {
            Sleep(5);
            if (!elems_queue.empty())
            {
                _InterlockedExchange(&flag, false);
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

        
    }
    _InterlockedExchange(&flag, false);
}

int main()
{

    srand(time(nullptr));

    
    std::future<void> task4[5];
    for (int i = 0; i < 5; ++i)
    {
        if (i < 2)
        {
            task4[i] = std::async(std::launch::async , &task_producer, i);
        }
        else
        {
            task4[i] = std::async(std::launch::async , &task_consumer, i);
        }
    }

    for (size_t i = 0; i < 5; i++)
    {
        task4[i].get();
    }
    
    std::cout << "Done.";
    /*while (!add_one.empty())
    {
        std::cout << add_one.front() << '\n';
        add_one.pop();
    }*/
    std::cin.get();
    return 0;
}