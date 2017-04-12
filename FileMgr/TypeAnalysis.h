#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h -Type analysis package                          //
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
This package creates a type table for a collection of files. The types which we have taken care as the part of 
this package is class,enum,struct, typedef,functions and alias. In the type table we are saving the type, the name
and the file in which it is present. We will be using this type table for the dependency analysis in the second
phase of the project.

Public Interface:
=================
setNullToCloseThread            // This method is to set the nullptr work item to shut the threads.
waitProcessor					// This method is to  join  the threads in threadpool.
startThread						// This method is to invoke the threads in thread pool.
getResultTable					// This method helps in fetching result from result queue.
execute							// This is overridden method of IFileHandler struct which creates work item.

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
#include "FileSystem.h"
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../QueuedWorkItems/Task.h"
#include "../QueuedWorkItems/TaskHelper.h"


struct TypeAnalysis : FileManager::IFileEventHandler {
public:
	void setNullToCloseThread() { task.setNullToCloseThread(); }
	void waitProcessor() { task.waitProcessor(); }
	void startThread() { task.startThread(); }
	std::unordered_multimap<std::string, std::string> getResultTable() { return task.getResult(); }
	void execute(const std::string& fileSpec);	

private:
	Task task;
	std::map<std::string, std::string> TypeTable;

};


#endif
