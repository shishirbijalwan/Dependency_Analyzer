#ifndef DEPENDENCYANALYSIS_H
#define DEPENDENCYANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  DependencyAnalysis.h - Dependency Analysis of file             //
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
- Tokenizer.h, Tokenizer.cpp
Build Command: devenv Project1.sln /rebuild debug

Maintenance History:
====================
ver 1.0

*/

#include <string>
#include <iostream>
#include <mutex>
#include <map>
#include "../FileMgr/FileMgr.h"
#include "../Tokenizer/Tokenizer.h"
class DependencyAnalysis {
public:
	DependencyAnalysis(std::map<std::string, std::string> tmap);
	void getDependencyAnalysis(std::string file);
private:
	std::map<std::string, std::string> typeTable;
	std::map<std::string, std::string> DependencyTable;
	


	
};


#endif
