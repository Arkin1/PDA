#include"semaphore.h"

void semaphore::up()
{
	unique_lock<mutex> lock(mtx);

	x++;

	lock.unlock();
	cond.notify_one();
}

void semaphore::down()
{
	unique_lock<mutex> lock(mtx);

	cond.wait(lock, [&] {
		if (x > 0)
			return true;
		else
			return false;
	});

	--x;


	lock.unlock();


}