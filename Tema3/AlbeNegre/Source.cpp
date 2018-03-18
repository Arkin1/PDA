
#include<iostream>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<unistd.h>
#include<time.h>
using namespace std;
#define FREE 3
#define AUX 2
#define BLACK 1
#define WHITE 0

int waiting[2] = {0,0}, used[2] = {0,0}, inUse[2] = {0,0}, user = FREE;

pthread_mutex_t waitMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t counter = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t waitCond[2] = {PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER};
string s[4] = {"alb", "negru", "aux", "free"};
void getResource(int type)
{
	sleep(1);
	while(true)
	{
		if(user == FREE)
		{
			user = type;
			cout<<"user is no longer free\n";
		}

		if(user == type)
		{
			pthread_mutex_lock(&counter);
			if(used[type] > 4 && waiting[!type])
			{
				user = AUX;
				waiting[type] ++;
				pthread_cond_wait(&waitCond[type],&counter);
				pthread_mutex_unlock(&counter);

			}
			else
			{
				cout<<"Process of type "<< s[type] <<" gets the resource\n";
				used[type]++;
				inUse[type]++;
				pthread_mutex_unlock(&counter);

				return ;
			}

			
		}
		else
		{
			pthread_mutex_lock(&waitMutex);

			waiting[type]++;
			pthread_cond_wait(&waitCond[type],&waitMutex);

			pthread_mutex_unlock(&waitMutex);
		}
	}
}

void useResource(int type)
{
	pthread_mutex_lock(&counter);
	cout<<"Current type: "<< s[type] << ", user: " << s[user] << ", used: " << used[type]<<endl;
	pthread_mutex_unlock(&counter);
	sleep(rand()%2+3);
}

void popResource(int type)
{
	sleep(1);
	pthread_mutex_lock(&counter);
	cout<<"Process of type "<< s[type] <<" pops the resource "<< inUse[type] << "\n";
	inUse[type]--;
	if(inUse[type] == 0 && user == AUX)
	{
		used[type] = 0;
		waiting[type^1] = 0;
		user = type^1;
		pthread_cond_broadcast(&waitCond[type^1]);
	}
	else 
	{
		if(inUse[type] == 0)
		{
			user = FREE;
			cout<<"user is now free\n";
		}
	}
	pthread_mutex_unlock(&counter);
}

void* proc(void *data)
{
	int el = *(int*)data;
	delete (int*)data;
	//cout<<"Process of type " << s[el] << " was created\n";
	getResource(el);
	useResource(el);
	popResource(el);
	return NULL;		
}

int main()
{
	srand(time(NULL));
	int i = 0;
	while(true)
	{
		sleep(1);

		int *p = new int;
		*p = rand()%2;
		pthread_t thr;
		pthread_create(&thr, NULL, proc, (void*)p);

		if(i++ == 10)
		{
			sleep(5);
		}
	}
 
    return 0;
}
