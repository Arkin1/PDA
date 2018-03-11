#include<iostream>
#include<fstream>
#include<algorithm>
#include<queue>
#include<thread>
#include<mutex>
#include<Windows.h>
#include"semaphore.h"

using namespace std;

queue<int> Q;
mutex mtx;
int maxSize=30;

semaphore full_sem(0), free_sem(30);

void produce()
{
	while (true)
	{
		int data = rand();

		free_sem.down();

		mtx.lock();

		Q.push(data);

		Sleep(3000);

		mtx.unlock();

		full_sem.up();


		cout << "S-a introdus " << data << "\n";
	}

}

void consume()
{
	while (true)
	{
		
		int data;

		full_sem.down();

		mtx.lock();

		data = Q.front();
		Q.pop();

		Sleep(2000);

		mtx.unlock();

		free_sem.up();

		cout << "S-a scos " << data << "\n";
	}
}

int main()
{
	thread producers[3];
	thread consumers[3];

	for (int i = 0; i < 3; ++i)
		producers[i] = thread(produce);

	for (int i = 0; i < 3; ++i)
		consumers[i] = thread(consume);

	for (int i = 0; i < 3; ++i)
		producers[i].join();

	for (int i = 0; i < 3; ++i)
		consumers[i].join();

	return 0;
}