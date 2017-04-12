#ifndef PARALLELDEPENDENCYEXECUTIVE_H
#define PARALLELDEPENDENCYEXECUTIVE_H
/////////////////////////////////////////////////////////////////////
//  ParallelDependencyExecutive.h - Executive file of Package      //
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
This package has been created to demonstrate all the requirement of the project 3 are been full filled.
We have devided this package into sub parts with the help of functions which take care of the requirements.
Any new files that needs to be tested on this package should be passed as command line argument and will
be displayed in Requirement 9 and 10 section of the displayed result.

Public Interface:
=================
createTypeTable                 // This is to create type table as part of requirement 9.
createDependencyTable			// This is to create the dependency table as part of requirement 9.
displayStartOfRequirement		// Display method related to requirement 1,2,3.
requirementThreadPoolDisplay	// To demonstarte  reuirement 5 and 6 are fullfilled.
requirementDependency			// To demonstarte  reuirement 7 and 8 are fullfilled.
getMainArument					// This is to save main method argument in member variable

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
#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include "../Utilities/Utilities.h"
#include "../QueuedWorkItems/QueuedWorkItems.h"
#include "../QueuedWorkItems/TaskHelper.h"



class ParallelDependencyExecutive {
public:
	void createTypeTable(std::vector<std::string> argumentList);
	void createDependencyTable(std::vector<std::string> argumentList);
	void displayStartOfRequirement();
	void requirementThreadPoolDisplay();
	void requirementDependency();
	void displayOfRequirementDependency();
	std::vector<std::string>& getMainArument() { return mainArgument; }

private:
	
	std::unordered_multimap<std::string, std::string> TypeTable;
	std::unordered_map<std::string, std::string> DependencyTable;
	std::vector<std::string> mainArgument;
};



#endif
