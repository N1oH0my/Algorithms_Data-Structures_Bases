#include <iostream>
#include <Windows.h>
#include <omp.h>

void f()
{
	std::cout /*<< "Thread"*/ << omp_get_thread_num() << '\n';
}

void fill_arr(int* arr, int size)
{
	srand(GetTickCount64());
	for (int i = 0; i < size; ++i)
		arr[i] = rand() % 100;
}
int non_parallel(int* arr, int size)
{
	int sum(0);
	for (int i = 0; i < size; ++i)
		sum += arr[i];
	return sum;
}

//int parallel(int* arr, int size)
//{
//	int global_sum(0);
//	//int i = 0;//��������
//#pragma omp parallel shared(arr), /*firstprivate(i),*/ reduction(+:global_sum)
//	{
//#pragma omp for //��������� ��� �������� ���������� ���� ������������ ����� ���������� ����������
//		for (int i = 0; i < size; ++i)
//		global_sum += arr[i];
//
//	}
//	return global_sum;
//}

//int parallel(int* arr, int size)
//{
//	int global_sum(0);
//	
//#pragma omp parallel shared(arr) /*firstprivate(i),*/
//	{
//		int local_sum(0);
//#pragma omp for //��������� ��� �������� ���������� ���� ������������ ����� ���������� ����������
//		for (int i = 0; i < size; ++i)
//		local_sum += arr[i];
//
//#pragma omp atomic//critical()
//		global_sum += local_sum;
//	}
//	return global_sum;
//}

//int parallel(int* arr, int size)
//{
//	int global_sum(0);
//
//#pragma omp parallel shared(arr) /*firstprivate(i),*/
//	{
//		int local_sum(0);
//#pragma omp for //��������� ��� �������� ���������� ���� ������������ ����� ���������� ����������
//		for (int i = 0; i < size; ++i)
//			local_sum += arr[i];
//
//#pragma omp critical(cout)
//		{
//			global_sum += local_sum;
//			std::cout << local_sum << '\n';
//		}
//	}
//	return global_sum;
//}

//int parallel(int* arr, int size)
//{
//	int global_sum(0);
//	omp_lock_t lock;
//	omp_init_lock(&lock);
//#pragma omp parallel shared(arr)
//	{
//		int local_sum(0);
//#pragma omp for
//		for (int i = 0; i < size; ++i)
//			local_sum += arr[i];
//		omp_set_lock(&lock);
//		global_sum += local_sum;
//		std::cout << local_sum << '\n';
//		//����� ��� �������� catch throw
//		omp_unset_lock(&lock);
//	}
//	omp_destroy_lock(&lock);
//	return global_sum;
//}

int parallel(int* arr, int size)
{
	int global_sum(0);
	omp_lock_t lock;
	omp_init_lock(&lock);
#pragma omp parallel shared(arr)
	{
		int local_sum(0);
#pragma omp for schedule(static, 10)//���� �� ������ �� 10//static(�� ����� �� ����), dynamic(�� ����� 1); ������� �������:  � ������� ���������� �������������� ���� �������, � � �������� ������� ������� ���������������(� �������� ��������)//������ �������� ��������� ������
		for (int i = 0; i < size; ++i)
			local_sum += arr[i];

		omp_set_lock(&lock);
		global_sum += local_sum;

		std::cout << local_sum << '\n';
		//����� ��� �������� catch throw
		omp_unset_lock(&lock);
		

	}
	omp_destroy_lock(&lock);
	return global_sum;
}

int main()
{
//������� ���������, ��� OpenMP

// ������ ���� �������
//#pragma omp master //�������������, �� ������ ���������, ��� ��� ����������� ������� �������
//	std::cout << "Master thread\n";
//#pragma omp parallel
//	{
//		std::cout /*<< "Thread"*/ << omp_get_thread_num()<<'\n';
//	}
//	std::cout << "Master thread\n";
//#pragma omp parallel
//	{
//		f();
//	}

//	��� ����������� ���������� �������
// int value = 123;
//#pragma omp master //�������������, �� ������ ���������, ��� ��� ����������� ������� �������
//	std::cout << "Master thread\n";
//#pragma omp parallel
//	{
//		value++;
//		std::cout << value << '\n';
//	}

//	int value = 123;
//#pragma omp parallel
//	{
//#pragma omp atomic//���������, ���������� �� �� ��� ���������� �������� ��������� ��������
//		value++;
//#pragma omp barrier
//
//#pragma omp critical//�������� ����������� ���� ��� ����� ������� ��������
//		{
//			std::cout <<"value:"<< value << '\n';
//		}
//	}

const int size = 105;
int arr[size];
fill_arr(arr, size);
std::cout << non_parallel(arr,size)<<'\n';
std::cout << parallel(arr, size) << '\n';
	

	std::cin.get();
	return 0;
}