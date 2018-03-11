#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include<mutex>
#include<condition_variable>
using namespace std;

class semaphore
{
private:
	int x;
	mutex mtx;
	condition_variable cond;
public:

	semaphore(int x)
	{
		this->x = x;
	}

	void up();

	void down();

};

#endif