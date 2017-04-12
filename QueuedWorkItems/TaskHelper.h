#ifndef TASKHELPER_H
#define TASKHELPER_H
/////////////////////////////////////////////////////////////////////
//  Task_Helper.h - Helper class for workitems                     //
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
This package is a helper package which helps in creation of the callable objects which can be inserted in the workitem
queue. Based on the bool flag we can make the functor act in different ways. In the current project by making this flag 
true we are craeting callable objects for Type Analysis and by sending false we are creating objects for parallel dependency
analysis. In future if we want to do any other work using threads we can just add conditions to make the functor work in those
condition.

Public Interface:
=================
()Functor            // This method is to create type table of dependency table based on requirement.

Build Process:
==============
Required files
- ParallelDependencyAnalaysis.h, TypeAnalysis.h, FileMgr.h, IFileMgr, Parser.h, Tokenizer.h, ConfigureParser.h,ActionRule.h,DependencyAanalysis.h, QueuedWorkItems.h
Build Command: devenv Shishir_Project3.sln /rebuild debug

Maintenance History:
====================
ver 1.0

*/
#include <map>
#include <unordered_map>

class TaskHelper {
public:
	TaskHelper(std::string file, std::unordered_multimap<std::string, std::string> table, bool flag);
	std::map<std::string, std::string> operator()();

private:
	std::string fileName;
	std::map<std::string, std::string> tempTypeTable;
	std::unordered_multimap<std::string, std::string> Phase1TypeTable;
	std::map<std::string, std::string> TempDependencyTable;
	bool TaskHelperFlag;


};


#endif

