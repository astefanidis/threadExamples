#include <iostream>
#include <thread>
#include <vector>
#include <cfloat>
#include <time.h>

#define NUM_THREADS 8

using namespace std;


//function run by the thread
/*
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
*/


void returnBiggestElement(vector<float> numbers, float* biggest, int start, int end)
{
	float maxNum = -FLT_MAX;
	for(int i=start; i<end; i++)
	{
		if(numbers[i]>maxNum)
		{
			maxNum = numbers[i];
		}
	}
	*biggest = maxNum;
}


float maxNumber(vector<float> &numbers)
{
	
	
	int vectorElementsPerThread = numbers.size()/NUM_THREADS;  //number of elements each thread will process
	int remainderElements = numbers.size() % NUM_THREADS;      //will also be allocated to threads
	int unusedThreads = 0;
	if(vectorElementsPerThread == 0)    //if we have more elements than threads, we can leave some threads unused
	{
		unusedThreads = NUM_THREADS - numbers.size();
	}
	vector<float> maxFromEachThread(NUM_THREADS-unusedThreads);  //will store the max number from each thread
	vector<thread> threads(NUM_THREADS-unusedThreads);           //the threads
	int start=0;  //first element the thread will process
	int end=0;    //last+1 element
	
	for(int i = 0; i<NUM_THREADS - unusedThreads; i++)
	{
		vector<float> tempElements;
		if(remainderElements == 0)
		{
			end = start + vectorElementsPerThread;
		}
		else
		{
			end = start + vectorElementsPerThread + 1; //add the remainder elements until they finish
			remainderElements--;
		}
		
		/*for(int j=start; j<end; j++)
		{
			tempElements.push_back(numbers[j]);
		}
		*/
		
		threads[i] = thread(returnBiggestElement,numbers,&maxFromEachThread[i],start,end);
		
		start = end;
	}
	for(int i=0; i<threads.size(); i++)
	{
		
		if(threads[i].joinable())
		{
			threads[i].join();
		}
	}
	
	float maxNum = -FLT_MAX;
	//find the global maximum
	for(auto i:maxFromEachThread)
	{
		if(i>maxNum)
		{
			maxNum = i;
		}
	}
	return maxNum;
	
}

float maxNumSer(vector<float> numbers)
{
	float biggest = -FLT_MAX;
	for(auto i:numbers)
	{
		if(i > biggest)
		{
			biggest = i;
		}
	}
	return biggest;
}

int main()
{
	vector<float> numbers(1000000000);
	for(int i=0; i<numbers.size(); i++)
	{
		numbers[i] = i*i;
	}
	float maxNum, maxNum2;
	clock_t start = clock();
	maxNum = maxNumber(numbers);
	
	
	clock_t end1 = clock();
	maxNum2 = maxNumSer(numbers);
	clock_t end2 = clock();
	cout << "serial: " << maxNum2 << " " << (float)(end2-end1)/CLOCKS_PER_SEC <<endl;
	cout << "parallel: " << maxNum << " " << (float)(end1-start)/CLOCKS_PER_SEC <<endl;
	return 0;
}