#include <iostream>
#include <queue>
#include <Windows.h>
#include <omp.h>
omp_lock_t lock;
omp_init_lock(&lock);
class ThreadSafeQueue
{
private:
    std::queue<int> queue;
public:
    void push(int elem, int ID)
    {
        omp_set_lock(&lock);
            queue.push(elem);
            std::cout << 'P' << ID << " -> " << elem << '\n';
        omp_unset_lock(&lock);
    }
    bool try_pop(int& elem, int ID)
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
                std::cout << 'C' << ID << " sleep\n";
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
ThreadSafeQueue TSQ;

int flag = 0;
int lock = 0;
int volume_work_producer = 20;
int volume_work_consumer = 20;

std::queue<int> addone;
void task_producer(int ID)
{
        // пока есть работа у потока-производителя
        while (volume_work_producer > 0)
        {

            int elem = rand() % 100;
            Sleep(2);
            TSQ.push(elem + ID, ID);

#pragma omp critical(notification)
            {
                flag = 1;
            }

#pragma omp critical(P)
            {
                volume_work_producer -= 1;
            }
        }
}
void task_consumer(int ID)
{
        while (volume_work_consumer > 0)
        {
            int elem;
            while (flag == 0)
            {
                Sleep(5);
#pragma omp critical(waitflag)
                {
                    flag = (int)!TSQ.empty();
                }
            }

            if (TSQ.try_pop(elem, ID))
            {
#pragma omp critical(addtooelem)
                {
                    elem += 1;
                }
                /*#pragma omp critical(pushqueue)
                {
                    addone.push(elem);
                }*/
                addone.push(elem);
            }
            else
            {
#pragma omp critical(nC)
                {
                    volume_work_consumer += 1;
                }
            }
#pragma omp critical(C)
            {
                volume_work_consumer -= 1;
            }
        }
}
int main()
{
    srand(GetTickCount());
#pragma omp parallel shared(addone)
    {
        #pragma omp for
            for (int i = 0; i < 5; ++i)
                if (i<2)
                {
                    task_producer(i);
                }
                else
                {
                    task_consumer(i);
                }
        
        #pragma omp barrier
    }

    while (!addone.empty())
    {
        std::cout << addone.front() << '\n';
        addone.pop();
    }
    std::cin.get();
    return 0;
}