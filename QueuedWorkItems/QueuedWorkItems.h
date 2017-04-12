#ifndef QUEUEDWORKITEMS_H
#define QUEUEDWORKITEMS_H
/////////////////////////////////////////////////////////////////////
//  QueuedWorkItems.h - Thread pool Package                        //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   Parallel Dependency Analysis, Sp16               //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This package is a thread pool. In this we are creating the threads based on the system hardware capacity using
hardware_concurrency. This thread pool accepts the work items which are object of Task_helper class and executes
the functor related to that object. It has functions to start, join and delete the threads.

Public Interface:
=================
start						  // This methods creates the threadpool and start dequeuing workitem.
doWork						  // This is to add work item to queue.
result						  // To poping out the result from queue .
wait						  // It uses the thread join condition to make thread wait.
getmax_Threads				  // It gives the number of threads generated.

Build Process:
==============
Required files
- ParallelDependencyAnalaysis.h, TypeAnalysis.h, FileMgr.h, IFileMgr, Parser.h, Tokenizer.h, ConfigureParser.h,ActionRule.h,DependencyAanalysis.h, QueuedWorkItems.h
Build Command: devenv Shishir_Project3.sln /rebuild debug

Maintenance History:
====================
ver 1.0

*/

#include <thread>
#include <functional>
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
#include "../QueuedWorkItems/TaskHelper.h"

using WorkItem = TaskHelper*;

///////////////////////////////////////////////////////////////////////
// class to process work items
// - each work item is processed sequentially on a single child thread

template<typename Result>
class ProcessWorkItem
{
public:
	void start();
	void doWork(WorkItem pWi);
	Result result();
	void wait();
	~ProcessWorkItem();
	int getmax_Threads();
private:
	std::vector <std::thread*> _pThreadVector ;
	BlockingQueue<WorkItem> _workItemQueue;
	BlockingQueue<Result> _resultsQueue;
	int max_Threads = std::thread::hardware_concurrency();

};
//----< wait for child thread to terminate >---------------------------

template<typename Result>
void ProcessWorkItem<Result>::wait()
{
	for (int i = 0; i < max_Threads; i++) {
		_pThreadVector[i]->join();

	}	

}
//----< enqueue work item >--------------------------------------------

template<typename Result>
void ProcessWorkItem<Result>::doWork(WorkItem pWi)
{
	_workItemQueue.enQ(pWi);
}
//----< retrieve results with blocking call >--------------------------

template<typename Result>
Result ProcessWorkItem<Result>::result()
{
	return _resultsQueue.deQ();
}
//----< start child thread that dequeus work items >-------------------

template<typename Result>
void ProcessWorkItem<Result>::start()
{

	std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			WorkItem pWi = _workItemQueue.deQ();
			if (pWi == nullptr)
			{
				return;
			}
			
			Result result = (*pWi)();
		
			_resultsQueue.enQ(result);
		}
	};
	for (int i = 0; i < max_Threads; i++) {
		_pThreadVector.push_back(new std::thread(threadProc));

	}
}
//----< clean up heap >------------------------------------------------

template<typename Result>
ProcessWorkItem<Result>::~ProcessWorkItem()
{
	for (int i = 0; i < max_Threads; i++) {
	delete	_pThreadVector[i];

	}
}
//------< Get max number of thread >-------------------------------
template<typename Result>
int ProcessWorkItem<Result>::getmax_Threads()
{
	return max_Threads;
}
#endif