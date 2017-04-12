/////////////////////////////////////////////////////////////////////
//  ParallelDependencyExecutive.cpp - Executive file of Package    //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   Parallel Dependency Analysis, Sp16               //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include <conio.h>
#include <map>
#include "../Utilities/Utilities.h"
#include "../FileMgr/TypeAnalysis.h"
#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
#include "ParallelDependencyExecutive.h"



#ifdef TEST_PARALLELDEPENDENCYEXECUTIVE
int main(int argc, char* argv[])
{
	ParallelDependencyExecutive  parallelDependencyExecutive;
	if(argc>2){
	for (int i = 1; i < argc; i++) {
		parallelDependencyExecutive.getMainArument().push_back(argv[i]);
			}
	}
	else {
		std::cout << "Please give atleast a File Path" << std::endl;
		return 0;
	}
	try {
	std::vector<std::string> cmdArgument;
	parallelDependencyExecutive.displayStartOfRequirement();
	parallelDependencyExecutive.requirementThreadPoolDisplay();
	parallelDependencyExecutive.requirementDependency();
	parallelDependencyExecutive.createTypeTable(cmdArgument);
	parallelDependencyExecutive.createDependencyTable(cmdArgument);
	}
	catch (const std::exception &e) { std::cout << e.what(); }


	return 0;
}

#endif
//This method displays the first part of requirement 9 and 10
void ParallelDependencyExecutive::createTypeTable(std::vector<std::string> argumentList)
{
	std::cout << "==========================================================================================================" << std::endl;
	std::cout << "Req 9 and 10: This requirement is covered in two parts. First Type Table is Show and then the dependency. " << std::endl;
	std::cout << "We have selected files with pattern .h and .cpp. This folder has .txt files which are not getting selected." << std::endl;
	std::cout << "===========================================================================================================" << std::endl;

	TypeAnalysis* taskAnalysis1 = new TypeAnalysis();
	//std::string path = "..\\Test_Files\\Test_Executive\\";

	FileManager::IFileMgr* pFmgr = FileManager::FileMgrFactory::create(mainArgument[0]);
	pFmgr->regForFiles(taskAnalysis1);
	for (int i = 1; i < mainArgument.size(); i++)
		pFmgr->addPattern(mainArgument[i]);
	pFmgr->search();	
	std::unordered_multimap<std::string, std::string> Result;
	Result = taskAnalysis1->getResultTable();
	std::cout << "==========================Requirement 10 part 1===========================================" << std::endl;
	for (std::unordered_multimap<std::string, std::string>::iterator it = Result.begin(); it != Result.end(); it++) {

		std::cout << "Type " << it->first << " In File  " << it->second << std::endl;

	}
	std::cout << "==========================Above is the TypeTable===========================================" << std::endl;
	std::cout << "**Please be patient this parts takes a bit of time" << std::endl;

	TypeTable = Result;

}
//This method displays the second part of requirement 9 and 10
void ParallelDependencyExecutive::createDependencyTable(std::vector<std::string> argumentList)
{
	//std::string path = "..\\Test_Files\\Test_Executive\\";
	ParallelDependencyAnalysis* ParallelDependencyAnalysis1 = new ParallelDependencyAnalysis(TypeTable);
	FileManager::IFileMgr* pFmgr2 = FileManager::FileMgrFactory::create(mainArgument[0]);
	for (int i = 1; i < mainArgument.size(); i++)
		pFmgr2->addPattern(mainArgument[i]);
	pFmgr2->regForFiles(ParallelDependencyAnalysis1);
	pFmgr2->search();
	
		
	std::unordered_multimap<std::string, std::string> ParallelDependencyAnalysisResult;
	std::unordered_multimap<std::string, std::string> ParallelDependencyAnalysisFinalResult;
	ParallelDependencyAnalysisResult = ParallelDependencyAnalysis1->getResultTable();
	
	for (std::unordered_multimap<std::string, std::string>::iterator it = ParallelDependencyAnalysisResult.begin(); it != ParallelDependencyAnalysisResult.end(); it++) {
		std::pair<std::string, std::string> temp(it->second, it->first);
		ParallelDependencyAnalysisFinalResult.insert(temp);
	}
		std::cout << "==========================Requirement 10 Part 2 starts===========================================" << std::endl;
	for (auto iter = ParallelDependencyAnalysisFinalResult.begin(); iter != ParallelDependencyAnalysisFinalResult.end(); iter = ParallelDependencyAnalysisFinalResult.equal_range(iter->first).second) {
		auto unique_key = iter->first;
		if (ParallelDependencyAnalysisFinalResult.count(unique_key) >0) { // if count is greater than 1, push the values to the result.
			std::cout << "----------------------------------------------------------------------------" << std::endl;
			std::cout << "File- " << iter->first << std::endl;
			auto it_bounds = ParallelDependencyAnalysisFinalResult.equal_range(unique_key);
			for (auto it = it_bounds.first; it != it_bounds.second; it++) {
				std::cout << "   dependent on- " << it->second << std::endl;

			}
		}
	}
	std::cout << "==========================Above is the Dependency Analysis===========================================" << std::endl;
	std::cout << "* Please note the files which you will pass for testing will be displayed in Req9 section." << std::endl;
}
//This method displays requirement 1,2,3
void ParallelDependencyExecutive::displayStartOfRequirement()
{
	std::cout << "==========================================================================================================================" << std::endl;
	std::cout << "Req 1: Have used Visual Studio 2015 and its C++ Windows Console Projects, as provided in the ECS computer labs. " << std::endl;
	std::cout << "==========================================================================================================================" << std::endl;
	std::cout << "Req 2: Have used the C++ standard library's streams for all I/O and new and delete for all heap-based memory management." << std::endl;
	std::cout << "==========================================================================================================================" << std::endl;
	std::cout << "Req 3: Have created all packages based on \"Single Responsiblity Principle\"." << std::endl;
	std::cout << "    3.1: Created threadpool package Queuedwork item for creating threads based on  hardware_concurrency" << std::endl;
	std::cout << "    3.2: Created Task Package, which acts wrapper for Thread pool and helps in inserting work items." << std::endl;
	std::cout << "    3.3: TypeAnalysis package is for creating the type table." << std::endl;
	std::cout << "    3.4: Dependency Analysis package if for doing dependency analysis on individual files." << std::endl;
	std::cout << "    3.5: Parallel dependency analysis  is for doing parallel dependency analysis of multiple file." << std::endl;
	std::cout << "    3.6: Parallel Dependency Executive is to test the complete setup." << std::endl;

	std::cout << "==========================================================================================================================" << std::endl;


}
//This method displays  requirement 4,5,6
void ParallelDependencyExecutive::requirementThreadPoolDisplay()
{
	std::cout << "====================================================================================================================================================" << std::endl;
	std::cout << "Req 4:Provided threadpool in package QueuedWorkItems that executes enqueued work items asynchronously. Demonstarted after requirement 7 " << std::endl;
	std::cout << "====================================================================================================================================================" << std::endl;
	std::cout << "Req 5:Created Task package that declares and defines a Task class that executes a callable object  with help of Task Helper class on a ThreadPool thread." << std::endl;
	std::cout << "====================================================================================================================================================" << std::endl;
	std::cout << "Req 6: TypeAnalysis package that identifies all of the types defined in a specified files." << std::endl;
	std::cout << "----------------------------------------------------------------------------" << std::endl;
	std::cout << "We will get the Type Tables for files in particular folder using thread pool and Task package." << std::endl;
	std::cout << "----------------------------------------------------------------------------" << std::endl;

	TypeAnalysis* pal = new TypeAnalysis();
	std::string path = "..\\Test_Files\\Req6\\";

	FileManager::IFileMgr* pFmgr = FileManager::FileMgrFactory::create(path);
	pFmgr->regForFiles(pal);
	pFmgr->search();

	std::unordered_multimap<std::string, std::string> Result;
	Result = pal->getResultTable();

	for (std::unordered_multimap<std::string, std::string>::iterator it = Result.begin(); it != Result.end(); it++) {

		std::cout << it->first << " In File  " << it->second << std::endl;
		
	}
}
//This method displays  Dependency Table
void ParallelDependencyExecutive::requirementDependency()
{
	displayOfRequirementDependency();
	std::unordered_multimap<std::string, std::string> typeTable;
	std::pair<std::string, std::string> pair1("Class->scanner::Toker", "file1");
	std::pair<std::string, std::string> pair2("Class->different::Toker", "file2");
	std::pair<std::string, std::string> pair3("Function->gamePlay", "file3");
	std::pair<std::string, std::string> pair4("Function->Toker", "file4");
	std::pair<std::string, std::string> pair5("Function->timeTable", "file5");
	typeTable.insert(pair1);typeTable.insert(pair2);typeTable.insert(pair3);typeTable.insert(pair4);typeTable.insert(pair5);

	std::string path = "..\\Test_Files\\Req78\\Inside_Folder1\\";
	ParallelDependencyAnalysis* pal = new ParallelDependencyAnalysis(typeTable);

	FileManager::IFileMgr* pFmgr = FileManager::FileMgrFactory::create(path);
	pFmgr->regForFiles(pal);
	pFmgr->search();

	std::unordered_multimap<std::string, std::string> Result;
	std::unordered_multimap<std::string, std::string> FinalResult;
	Result = pal->getResultTable();

	for (std::unordered_multimap<std::string, std::string>::iterator it = Result.begin(); it != Result.end(); it++) {

		std::pair<std::string, std::string> temp(it->second, it->first);
		FinalResult.insert(temp);
		}
	for (auto iter = FinalResult.begin(); iter != FinalResult.end(); iter = FinalResult.equal_range(iter->first).second) {
		auto unique_key = iter->first;
		if (FinalResult.count(unique_key) >0) { 
			std::cout << "File- " << iter->first << std::endl;
			auto it_bounds = FinalResult.equal_range(unique_key);
			for (auto it = it_bounds.first; it != it_bounds.second; it++) {
				std::cout << "   dependent on- " << it->second << std::endl;
			}
		}
	}
	std::cout << "** By seeing first dependency we can see it is able to differentiate between function Toker and class Toker." << std::endl;
	std::cout << "** By seeing second and third dependency I  demonstrated that i have taken care of same class name in different namespaces." << std::endl;
}
// This is just a display methods which shows the requirements
void ParallelDependencyExecutive::displayOfRequirementDependency()
{
	std::cout << "====================================================================================================================================================" << std::endl;
	std::cout << "Req 7:Provided DependencyAnalysis package that identifies all of the dependencies between files in a specified file collection " << std::endl;
	std::cout << "====================================================================================================================================================" << std::endl;
	std::cout << "Req 8: Provied ParallelDependencyAnalysis package that finds compilation dependencies between all of the files in a specified file collection using the asynchronous processing facilities provided by the Task package." << std::endl;
	std::cout << "----------------------------------------------------------------------------" << std::endl;
	std::cout << "For the above requirement, we have used below Type table." << std::endl;
	std::cout << "Class->scanner::Toker   File file1" << std::endl;
	std::cout << "Class->different::Toker File file2" << std::endl;
	std::cout << "Function->gamePlay      File file3" << std::endl;
	std::cout << "Function->Toker         File file4" << std::endl;
	std::cout << "Function->timeTable     File file5" << std::endl;
	std::cout << "----------------------------------------------------------------------------" << std::endl;

	
}
