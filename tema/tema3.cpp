#include<iostream>
#include<fstream>
#include<algorithm>
#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<Windows.h>
using namespace std;
 
queue<int> Q;
mutex mtx;
int maxSize = 30;
condition_variable condProd, condCons;
 
void produce()
{
    while (true)
    {
        int data = rand();
 
 
        unique_lock<mutex> lock(mtx);
 
        if(Q.size() < maxSize)
            condProd.wait(lock);
 
        Q.push(data);
 
        Sleep(3000);
 
        lock.unlock();
        condCons.notify_one();
 
        cout << "S-a introdus " << data << "\n";
    }
 
}
 
void consume()
{
    while (true)
    {
 
        int data;
 
 
        unique_lock<mutex> lock(mtx);
 
        if (Q.size() > 0)
            condCons.wait(lock);
 
        data = Q.front();
        Q.pop();
 
        Sleep(1000);
 
        lock.unlock();
 
        condProd.notify_one();
 
        cout << "S-a scos " << data << "\n";
    }
}
 
int main()
{
    thread producers[20];
    thread consumers[3];
 
    for (int i = 0; i < 3; ++i)
    {
        producers[i] = thread(produce);
        consumers[i] = thread(consume);
    }
       
 
    for (int i = 0; i < 3; ++i)
        producers[i].join();
 
    for (int i = 0; i < 3; ++i)
        consumers[i].join();
 
    return 0;
}
