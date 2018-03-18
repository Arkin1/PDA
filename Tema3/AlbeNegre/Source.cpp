#include<iostream>
#include<stdlib.h>
#include<mutex>
#include<thread>
#include<condition_variable>
#include<time.h>
#include<Windows.h>
#include<string>
using namespace std;
#define FREE 3
#define AUX 2
#define BLACK 1
#define WHITE 0

int waiting[2] = { 0,0 }, used[2] = { 0,0 }, inUse[2] = { 0,0 }, user = FREE;

mutex waitMutex;
mutex counter;
condition_variable waitCond[2];
string s[4] = { "alb", "negru", "aux", "free" };
void getResource(int type)
{
	Sleep(1000);
	while (true)
	{
		if (user == FREE)
		{
			user = type;
			cout << "user is no longer free\n";
		}

		if (user == type)
		{
			unique_lock<mutex> lock(counter);
			if (used[type] > 4 && waiting[!type])
			{
				user = AUX;
				waiting[type] ++;
				waitCond[type].wait(lock);
				lock.unlock();

			}
			else
			{
				cout << "Process of type " << s[type] << " gets the resource\n";
				used[type]++;
				inUse[type]++;
				lock.unlock();

				return;
			}


		}
		else
		{
			unique_lock<mutex> lock(waitMutex);

			waiting[type]++;
			waitCond[type].wait(lock);

			lock.unlock();
		}
	}
}

void useResource(int type)
{
	unique_lock<mutex> lock(counter);
	cout << "Current type: " << s[type] << ", user: " << s[user] << ", used: " << used[type] << endl;
	lock.unlock();
	Sleep(rand() % 2000 + 3000);
}

void popResource(int type)
{
	Sleep(1000);
	unique_lock<mutex> lock(counter);
	cout << "Process of type " << s[type] << " pops the resource " << inUse[type] << "\n";
	inUse[type]--;
	if (inUse[type] == 0 && user == AUX)
	{
		used[type] = 0;
		waiting[type ^ 1] = 0;
		user = type ^ 1;
		waitCond[type ^ 1].notify_all();
	}
	else
	{
		if (inUse[type] == 0)
		{
			user = FREE;
			cout << "user is now free\n";
		}
	}
	lock.unlock();
}

void proc(int data)
{
	getResource(data);
	useResource(data);
	popResource(data);
	
}

int main()
{
	srand(time(NULL));
	int i = 0;
	while (true)
	{
		Sleep(1000);

		
		int p = rand() % 2;
		thread(proc, p);

		if (i++ == 10)
		{
			Sleep(5000);
		}
	}

	return 0;
}
