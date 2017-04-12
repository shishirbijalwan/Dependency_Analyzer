#ifndef TASKHELPER_H
#define TASKHELPER_H
/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h - Analyzes C++ language constructs            //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   CodeParser for CSE687 Pr2, Sp16                 //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module is for doing the matrix analysis of the Abstract Syntax tree. It works
on the abstract syntax stree to calculate its complexity and line count.

Public Interface:
=================
DisplayMetrics (_type);          // display the complexity and lines of a particular type

Build Process:
==============
Required files
- ASTree.h

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
#include "../../Project2/Parser/Parser.h"
#include "../../Project2/Parser/ConfigureParser.h"
#include "../QueuedWorkItems/Task.h"


class TaskHelper {
public:
	TaskHelper(std::string file) { fileName = file; };
	std::map<std::string, std::string> operator()();

private:
	std::string fileName;
	std::map<std::string, std::string> tempTypeTable;
	
};


#endif

