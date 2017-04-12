#ifndef PARALLELDEPENDENCYANALYSIS_H
#define PARALLELDEPENDENCYANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  ParallelDependencyAnalysis.h - Parallel Dependency Analysis    //
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
This Package is to analyse the dependencies on a files over other files.This is done with
the help of threading. We used the pakage Dependency analysis for doing the analysis on many
files parallely. We enqueue functors in work item queue to achive the task of parallel processing.
We take help of the file manager package to call the execute method of this package.

Public Interface:
=================
setNullToCloseThread            // This method is to set the nullptr work item to shut the threads.
waitProcessor					// This method is to  join  the threads in threadpool.
startThread						// This method is to invoke the threads in thread pool.
getResultTable					// This method helps in fetching result from result queue.
execute							// This is overridden method of IFileHandler struct which creates work item.
ParallelDependencyAnalysis		// Constructor to initialize type table

Build Process:
==============
Required files
- ParallelDependencyAnalaysis.h, TypeAnalysis.h, FileMgr.h, IFileMgr, Parser.h, Tokenizer.h, ConfigureParser.h,ActionRule.h,DependencyAanalysis.h, QueuedWorkItems.h
Build Command: devenv Shishir_Project3.sln /rebuild debug

Maintenance History:
====================
ver 1.0

*/

#include <string>
#include <iostream>
#include <vector>
#include <mutex>
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../QueuedWorkItems/Task.h"
#include "../QueuedWorkItems/TaskHelper.h"
#include "../FileMgr/FileSystem.h"




struct ParallelDependencyAnalysis : FileManager::IFileEventHandler {
public:
	ParallelDependencyAnalysis(std::unordered_multimap<std::string, std::string> table);
	void setNullToCloseThread() { task.setNullToCloseThread(); }
	void waitProcessor() { task.waitProcessor(); }
	void startThread() { task.startThread(); }
	std::unordered_multimap<std::string, std::string> getResultTable() { return task.getResult(); }
	void execute(const std::string& fileSpec);
	

private:
	Task task;
	std::unordered_multimap<std::string, std::string> TypeTable;
	std::unordered_map<std::string, std::string> CollectiveDependencyAnalysisTable;
	
};


#endif
