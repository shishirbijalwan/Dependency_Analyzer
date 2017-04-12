/////////////////////////////////////////////////////////////////////
//  DependencyAnalysis.cpp - Dependency Analysis of file           //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   Parallel Dependency Analysis, Sp16               //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <fstream>
#include <exception>
#include <cctype>
#include <conio.h>
#include "../Utilities/Utilities.h"
#include "DependencyAnalysis.h"
#include "../Tokenizer/Tokenizer.h"
using namespace Scanner;



#ifdef TEST_DEPENDENCYANALYSIS
int main()
{
	std::unordered_multimap<std::string, std::string> typeTable;
	std::pair<std::string, std::string> pair1("Class->scanner::Toker", "file1");
	std::pair<std::string, std::string> pair2("Class->different::Toker", "file2");
	std::pair<std::string, std::string> pair3("Function->gamePlay", "file3");
	std::pair<std::string, std::string> pair4("Function->Toker", "file4");
	std::pair<std::string, std::string> pair5("Function->timeTable", "file5");
	typeTable.insert(pair1);
	typeTable.insert(pair2);
	typeTable.insert(pair3);
	typeTable.insert(pair4);
	typeTable.insert(pair5);

	std::string fileName = "..\\Test_Files\\Req78\\Inside_Folder1\\OnlyFunction.txt";
	DependencyAnalysis dependencyAnalysis(typeTable, fileName);
	dependencyAnalysis.findDependecy();
	
	std::map<std::string, std::string> Result;
	Result = dependencyAnalysis.gettempDependencyTable();
	std::cout << "Length of result is" << Result.size() << std::endl;
	for (std::map<std::string, std::string>::iterator it = Result.begin(); it != Result.end(); it++) {
		std::cout << "File- " << it->second << std::endl;
		std::cout << "   dependent on- " << it->first << std::endl;
  

	}
	return 0;
}

#endif

// This is the main function for finding dependency which uses other sub methods to achieve objective
std::map<std::string, std::string> DependencyAnalysis::findDependecy()
{
	std::ifstream in(fileName);
		Toker toker;
		toker.attach(&in);
		setdependencyTypeTable();
		
		do
		{
			std::string tok = toker.getTok();
			if (tok == "\n") { continue; }
			for (std::unordered_multimap<std::string, std::vector<std::string>>::iterator it = dependencyTypeTable.begin(); it != dependencyTypeTable.end(); it++) {
				key = it->first;
				value = it->second[0];
				type = it->second[1];
				if (value == fileName)
					continue;
				nameSpaceHelper(tok);					
			
			if (nameSpaceVector.size()==0) {// In case the file does not define namespace
				testWhenNoNameSpace(tok);
				if (tok == key && type == "Function") {
					std::string temp = toker.getTok();
					addHelper(temp);
					
				}
			}
			else {
				testWhenNameSpaceGiven(tok);
				if (tok == key && type == "Function") {
					std::string temp = toker.getTok();
					addHelper(temp);	
				}
			}

			lastTok = tok;
			}
		} while (in.good());


	return tempDependencyTable;
}


// This is a helper function which helps in analysis when no namespace is given
void DependencyAnalysis::testWhenNoNameSpace(std::string token)
{
	if (token != "::") 
		if (token==key && type!="Function") {
			std::pair<std::string, std::string> element(value, fileName);
			tempDependencyTable.insert(element);
		}
}
// This is a helper function which helps in analysis when  namespace is given
void DependencyAnalysis::testWhenNameSpaceGiven(std::string token)
{
	if (checkSubString("::")) {
		for (int i = 0; i < nameSpaceVector.size(); i++) {
		
			std::string testName = nameSpaceVector[i] + "::";
			testName = testName + token;

			if (testName == key) {
				std::pair<std::string, std::string> element(value, fileName);
				tempDependencyTable.insert(element);
				break;
			}

		}
	}
	else {
		if (token == key && type!="Function") {
			std::pair<std::string, std::string> element(value, fileName);
			tempDependencyTable.insert(element);			

		}
	}
}
// This is a helper methods for dependency analysis which returns bool based on substring found or not
bool DependencyAnalysis::checkSubString(std::string sub)
{
	size_t found = key.find(sub);
	if (found != std::string::npos)
	{
		//do stuff if word is there
		return true;
	}
	else
	{
		//do stuff when word isnt there
		return false;
	}
}

// This method helps in  separating the type which helps in reducing the complexity of Analysis and differentiate
//between function and object
void DependencyAnalysis::setdependencyTypeTable()
{
	for (std::unordered_multimap<std::string, std::string>::iterator it = typeTable.begin(); it != typeTable.end(); it++) {
		std::string varToBreak = it->first;
		std::string VectorFirstValue = it->second;
		size_t i;
		for (i = 0; i < varToBreak.length(); i++) {

			if (varToBreak[i] == '-')
				break;

		}
		std::string  VectorVectorSecondValue = varToBreak.substr(0, i);
		std::string  newKey = varToBreak.substr(i + 2, varToBreak.length() - 1);
		std::vector<std::string> vec;
		vec.push_back(VectorFirstValue);
		vec.push_back(VectorVectorSecondValue);
		dependencyTypeTable.insert(std::pair<std::string,std::vector<std::string>>(newKey, vec));
	}
}
// This function helps in saving the namespace in the file
void DependencyAnalysis::nameSpaceHelper(std::string tok)
{
	if (lastTok == "namespace") {
		nameSpaceVector.push_back(tok);
	}
}
//This function helps in adding a entry in dependency table
void DependencyAnalysis::addHelper(std::string temp)
{	
	if (temp == "(") {
			std::pair<std::string, std::string> element(value, fileName);
			tempDependencyTable.insert(element);
		}
}


