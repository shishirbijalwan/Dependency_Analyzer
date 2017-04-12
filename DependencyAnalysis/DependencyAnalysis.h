#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  DependencyAnalysis.h - Dependency Analysis of file             //
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
This Package is to analyse the dependencies on a file over other file. For this we have used a 
type table to be compared with the tokens provided by the tokenizer over that file. Based on 
various conditions checks like function check,Namespace check, struct,enum etc we create the 
table which has the information about the dependencies of file.

Public Interface:
=================
findDependecy						// This method is to check the dependency of a file over other files.
testWhenNoNameSpace					// This method helps findDependecy function when no namespace is given in file.
testWhenNameSpaceGiven			    // This method helps findDependecy function when  namespace is given in file.
checkSubString						// This method true or false depending on substring found or not.
gettempDependencyTable				// Getter method for temporary dependency table.
setdependencyTypeTable				// Setter method to set the TypeTable in DpendencyAnalysis class
nameSpaceHelper						// This method helps in setting the namespace found while tokenizing the file.
addHelper							// This is a helper method which helps in adding a entry in dependency table.

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
#include <vector>
#include <unordered_map>
#include "../Tokenizer/Tokenizer.h"


class DependencyAnalysis {
public:
	DependencyAnalysis(std::unordered_multimap<std::string, std::string> typetablemap, std::string file) { typeTable = typetablemap; fileName = file; }
	std::map<std::string, std::string> findDependecy();
	void testWhenNoNameSpace(std::string token);
	void testWhenNameSpaceGiven(std::string token);
	bool checkSubString(std::string sub);
	std::map<std::string, std::string> gettempDependencyTable() {return tempDependencyTable;}
	void setdependencyTypeTable();
	void nameSpaceHelper(std::string tok);
	void addHelper(std::string tok);
private:
	std::unordered_multimap<std::string, std::string> typeTable;
	std::map<std::string, std::string> tempDependencyTable;
	std::vector<std::string> nameSpaceVector;
	std::string lastTok;
	std::string secondLastTok;
	std::string fileName;
	std::string key;
	std::string value;
	std::string type;
	std::unordered_map<std::string, std::vector<std::string>> dependencyTypeTable;

};


#endif

#pragma once


