#include <iostream>
#include <thread>
#include <vector>
#include <cfloat>
#define NUM_THREADS 8

using namespace std;


//function run by the thread
void returnBiggestElement(vector<float> numbers, float* biggest)
{
	float maxNum = -FLT_MAX;
	for(auto i:numbers)
	{
		if(i>maxNum)
		{
			maxNum = i;
		}
	}
	*biggest = maxNum;
}

float maxNumber(vector<float> &numbers)
{
	
	
	int vectorElementsPerThread = numbers.size()/NUM_THREADS;
	int remainderElements = numbers.size() % NUM_THREADS;
	int unusedThreads = 0;
	if(vectorElementsPerThread == 0)
	{
		unusedThreads = NUM_THREADS - numbers.size();
	}
	vector<float> maxFromEachThread(NUM_THREADS-unusedThreads);
	vector<thread> threads(NUM_THREADS-unusedThreads);
	int start=0;
	int end=0;
	
	for(int i = 0; i<NUM_THREADS - unusedThreads; i++)
	{
		vector<float> tempElements;
		if(remainderElements == 0)
		{
			end = start + vectorElementsPerThread;
		}
		else
		{
			end = start + vectorElementsPerThread + 1; 
			remainderElements--;
		}
		cout << "thread: " << i << "start: " << start << " end: " << end << endl;
		for(int j=start; j<end; j++)
		{
			tempElements.push_back(numbers[j]);
		}
		
		//thread t(returnBiggestElement,tempElements,&localMax);
		threads[i] = thread(returnBiggestElement,tempElements,&maxFromEachThread[i]);
		//maxFromEachThread[i] = localMax;
		//threads.push_back(move(t));
		start = end;
	}
	for(int i=0; i<threads.size(); i++)
	{
		if(true)//threads[i].joinable())
		{
			threads[i].join();
		}
	}
	for(auto i:maxFromEachThread)
	{
		cout << i << endl;
	}
	float maxNum = -FLT_MAX;
	for(auto i:maxFromEachThread)
	{
		if(i>maxNum)
		{
			maxNum = i;
		}
	}
	return maxNum;
	
}


int main()
{
	vector<float> numbers(100);
	for(int i=0; i<numbers.size(); i++)
	{
		numbers[i] = -i*i;
	}
	float maxNum = maxNumber(numbers);
	cout << maxNum << endl;
	return 0;
}