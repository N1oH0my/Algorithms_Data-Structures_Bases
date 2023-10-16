#include <iostream>
#include <queue>
#include <Windows.h>
#include <time.h>
#include<random>
//--------------//
#include <omp.h>

class SafeQueue
{
private:
    omp_lock_t lock;
    std::queue<int> queue;
public:
    SafeQueue() { omp_init_lock(&lock); }
    ~SafeQueue() { omp_destroy_lock(&lock); }
    void push(int elem, int ID)
    {
        omp_set_lock(&lock);
        queue.push(elem);
        std::cout << 'P' << ID << " -> " << elem << '\n';
        omp_unset_lock(&lock);
    }
    bool pop(int& elem, int ID)
    {
        bool result = false;
        omp_set_lock(&lock);
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
        omp_unset_lock(&lock);
        return result;
    }
    bool empty()
    {
        bool isempty;
        omp_set_lock(&lock);
        isempty = queue.empty();
        omp_unset_lock(&lock);
        return isempty;
    }
};
SafeQueue elems_queue;

int flag = 0;
int work_for_producer = 50;
int work_for_consumer = 50;

std::queue<int> add_one;
void task_producer(int ID)
{
    // пока есть работа у потока-производителя
    while (work_for_producer > 0)
    {

        int elem = rand() % 100;
        Sleep(2);
        elems_queue.push(elem + ID, ID);


        flag = 1;
        

#pragma omp critical(P)
        {
            work_for_producer -= 1;
        }
    }
}
void task_consumer(int ID)
{
    while (work_for_consumer > 0)
    {
        int elem;
        while (flag == 0)
        {
            Sleep(5);
            flag = !elems_queue.empty() ? 1 : 0;
            
        }

        if (elems_queue.pop(elem, ID))
        {
            Sleep(5);
        }
        else
        {
#pragma omp critical(C)
            {
                work_for_consumer += 1;
            }
        }
#pragma omp critical(C)
        {
            work_for_consumer -= 1;
        }
    }

    flag = 1;
    
}

int main()
{

    srand(GetTickCount());
#pragma omp parallel shared(flag)
    {
    #pragma omp for 
        for (int i = 0; i < 5; ++i)
            if (i < 2)
            {
                task_producer(i);
            }
            else
            {
                task_consumer(i);
            }

    #pragma omp barrier
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