#include<iostream>
#include<fstream>
#include<algorithm>
#include<queue>
#include<thread>
#include<mutex>

using namespace std;

queue<int> Q;
mutex mtx;
int maxSize=30;
void produce()
{
	while (true)
	{
		mtx.lock();


		if (Q.size() < maxSize)
		{
			Q.push(2);

			cout << "S-a introdus 2\n";
		}

		mtx.unlock();
	}
}

void consume()
{
	while (true)
	{
		mtx.lock();

		if (Q.size())
		{
			int x = Q.front();
			Q.pop();
			cout << "S-a scos " << x << "\n";
		}

		mtx.unlock();
	}
}

int main()
{
	thread tr1(consume);
	thread tr2(produce);

	tr1.join();
	tr2.join();
	return 0;
}
