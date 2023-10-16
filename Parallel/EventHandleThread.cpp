#include <iostream>
#include <queue>
#include <Windows.h>
//--------------//
#include <thread>
//#include <process.h>

class SafeQueue
{
private:
    HANDLE hELock;
    std::queue<int> queue;
public:
    SafeQueue() { hELock = CreateEvent(nullptr, false, true, nullptr); }
    ~SafeQueue() { CloseHandle(hELock); }
    void push(int elem, int ID)
    {
        if (WaitForSingleObject(hELock, INFINITE) == WAIT_OBJECT_0)
        {
            queue.push(elem);
            std::cout << 'P' << ID << " -> " << elem << '\n';
            SetEvent(hELock);
        }
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        if (WaitForSingleObject(hELock, INFINITE) == WAIT_OBJECT_0)
        {
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
        }
        SetEvent(hELock);
        return result;
    }
    bool empty()
    {
        bool isempty;
        if (WaitForSingleObject(hELock, INFINITE) == WAIT_OBJECT_0)
        {
            isempty = queue.empty();
        }
        SetEvent(hELock);
        return isempty;
    }
};
SafeQueue elems_queue;

HANDLE flag;
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

        SetEvent(flag);

        _InterlockedExchangeAdd(&work_for_producer, -1);

    }
}
void task_consumer(int ID)
{
    while (work_for_consumer > 0)
    {
        int elem;
        while (WaitForSingleObject(flag, 5) != WAIT_OBJECT_0)
        {
            Sleep(5);
            if (!elems_queue.empty() || WaitForSingleObject(flag, 1) == WAIT_OBJECT_0)
            {
                SetEvent(flag);
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
    SetEvent(flag);
}

int main()
{

    srand(GetTickCount());

    flag = CreateEvent(nullptr, false, true, nullptr);
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
    CloseHandle(flag);
    std::cout << "Done.";
    /*while (!add_one.empty())
    {
        std::cout << add_one.front() << '\n';
        add_one.pop();
    }*/
    std::cin.get();
    return 0;
}