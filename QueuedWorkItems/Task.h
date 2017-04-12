#ifndef TASK_H
#define TASK_H
/////////////////////////////////////////////////////////////////////
//  Task.h - Wrapper package for Thread pool                       //
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
This package is for hiding the complexity of the thread pool from the user. It acts as the interface or wrapper.
It helps in equeuing the work items to the work item queue. It uses the the Task Helper functor for this task.
This a generic package which can be used by any system to do a task by making changes in the functor of the
of the task helper class. 

Public Interface:
=================
setNullToCloseThread            // This method is to set the nullptr work item to shut the threads.
waitProcessor					// This method is to  join  the threads in threadpool.
startThread						// This method is to invoke the threads in thread pool.
getResultTable					// This method helps in fetching result from result queue.
getTaskCount					// To get the task count for closing the thread count 
ResultForDependencyAnalysis		// This method is to return the resut from Result queue

Build Process:
==============
Required files
- ParallelDependencyAnalaysis.h, TypeAnalysis.h, FileMgr.h, IFileMgr, Parser.h, Tokenizer.h, ConfigureParser.h,ActionRule.h,DependencyAanalysis.h, QueuedWorkItems.h
Build Command: devenv Shishir_Project3.sln /rebuild debug

Maintenance History:
====================
ver 1.0

*/

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include "../Utilities/Utilities.h"
#include "../QueuedWorkItems/QueuedWorkItems.h"
#include "../QueuedWorkItems/TaskHelper.h"

class Task {
	public:
		void setNewTask(TaskHelper* pWi);
		void startThread() { processor.start(); }
		std::unordered_multimap<std::string, std::string> getResult();
		void waitProcessor() { processor.wait(); }
		int getTaskCount() { return taskCount; }
		void setNullToCloseThread();
		std::unordered_map<std::string, std::string> ResultForDependencyAnalysis();

private:
	ProcessWorkItem<std::map<std::string, std::string>> processor;
	int taskCount=0;
	std::unordered_multimap<std::string, std::string> FinalResult;
	std::unordered_map<std::string, std::string> FinalForDependencyTable;
};



#endif
