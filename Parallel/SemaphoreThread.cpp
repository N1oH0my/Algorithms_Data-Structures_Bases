#include <iostream>
#include <queue>
#include <Windows.h>
//--------------//
#include <thread>
//#include <process.h>
//#include <semaphore>
#include<condition_variable>
#include <mutex>
class Sem {
public:
    explicit Sem() {};

    void lock() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (count_ == 0) {
            c.wait(lock);
        }
        count_=0;
    }

    void unlock() {
        //std::unique_lock<std::mutex> lock(mutex_);
        count_=1;
        c.notify_all();
    }

private:
    int count_=1;
    std::mutex mutex_;
    std::condition_variable c;
};



class SafeQueue
{
private:
    Sem alock;
    std::queue<int> queue;
public:
    SafeQueue() {  }
    ~SafeQueue() {  }
    void push(int elem, int ID)
    {
        alock.lock();
        queue.push(elem);
        std::cout << 'P' << ID << " -> " << elem << '\n';
        alock.unlock();
        
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        alock.lock();
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
        alock.unlock();
        return result;
    }
    bool empty()
    {
        bool isempty;
        alock.lock();
        isempty = queue.empty();
        alock.unlock();
        return isempty;
    }
};
SafeQueue elems_queue;

volatile long flag = 0;
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

        _InterlockedExchange(&flag, false);

        _InterlockedExchangeAdd(&work_for_producer, -1);

    }
}
void task_consumer(int ID)
{
    while (work_for_consumer > 0)
    {
        int elem;

        while (_InterlockedExchange(&flag, true))
        {
            Sleep(5);
            if (!elems_queue.empty() || !flag)
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

        _InterlockedExchangeAdd(&work_for_consumer, -1);
    }
    _InterlockedExchange(&flag, false);
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