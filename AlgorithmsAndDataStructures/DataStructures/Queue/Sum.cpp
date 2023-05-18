#include <iostream>
#include <queue>
#include <Windows.h>
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


SafeQueue elems_queue;
int global_sum = 0;
int main()
{
    srand(GetTickCount());

    int N = 100;
    int* arr = new int[N];
    int n = N;
    while (n>0)
    {
        int elem = rand() % 100;
        Sleep(2);
        arr[n - 1] = elem;
        elems_queue.push(elem, n);
        --n;
    }

#pragma omp parallel
    {
        int cur_elem = 0;
        while (!elems_queue.empty())
        {
            elems_queue.pop(cur_elem, omp_get_thread_num());
        #pragma omp critical(sum)
            {
                global_sum += cur_elem;
            }
        }

    #pragma omp barrier
    }
    int check = 0;
    for (size_t i = 0; i < N; ++i)
    {
        check += arr[i];
    }
    std::cout << '\n' << check<<'\n';
    std::cout << '\n' << global_sum << '\n';
    std::cin.get();
    return 0;
}