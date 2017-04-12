/////////////////////////////////////////////////////////////////////
//  ParallelDependencyAnalysis.cpp - Parallel Dependency Analysis  //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   Parallel Dependency Analysis, Sp16               //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "../QueuedWorkItems/TaskHelper.h"
#include "ParallelDependencyAnalysis.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"

#ifdef TEST_DEPENDENCYANALYSIS



int main(int argc, char* argv[])
{	std::unordered_multimap<std::string, std::string> typeTable;
	std::pair<std::string, std::string> pair1("scanner::Toker", "file1");
	std::pair<std::string, std::string> pair2("different::Toker", "file2");
	std::pair<std::string, std::string> pair3("gamePlay", "file3");
	std::pair<std::string, std::string> pair4("SemiExpression", "file4");
	std::pair<std::string, std::string> pair5("timeTable", "file5");
	typeTable.insert(pair1);typeTable.insert(pair2);typeTable.insert(pair3);typeTable.insert(pair4);typeTable.insert(pair5);
	std::string path = "C:\\Users\\Megha\\Desktop\\RealProject_3\\Try1\\Project2\\Test_Files\\Folder1\\Inside_Folder1\\";
	//FileManager::IFileEventHandler* pal = new ParallelDependencyAnalysis(typeTable);
	ParallelDependencyAnalysis* pal = new ParallelDependencyAnalysis(typeTable);
	FileManager::IFileMgr* pFmgr= FileManager::FileMgrFactory::create(path);
	pFmgr->regForFiles(pal);
	pFmgr->search();
	std::unordered_multimap<std::string, std::string> Result;
	std::unordered_multimap<std::string, std::string> FinalResult;
	Result=pal->getResultTable();
	for (std::unordered_multimap<std::string, std::string>::iterator it = Result.begin(); it != Result.end(); it++) {

		std::pair<std::string, std::string> temp(it->second, it->first);
		FinalResult.insert(temp);
		//std::cout << "File- " << it->second << std::endl;
		//std::cout << "   dependent on- " << it->first << std::endl;
	}	
	for (auto iter = FinalResult.begin(); iter != FinalResult.end(); iter = FinalResult.equal_range(iter->first).second) {
		auto unique_key = iter->first;
		if (FinalResult.count(unique_key) >0) { // if count is greater than 1, push the values to the result.
			std::cout<< "File- " << iter->first << std::endl;
			auto it_bounds = FinalResult.equal_range(unique_key);
			for (auto it = it_bounds.first; it != it_bounds.second; it++) {
					std::cout << "   dependent on- " << it->second << std::endl;
			}
		}
	}

	return 0;
}

#endif
// This constructor is to initialize the typeTable variable of this package
ParallelDependencyAnalysis::ParallelDependencyAnalysis(std::unordered_multimap<std::string, std::string> table)
{
	TypeTable = table;

}
// This method enqueue the functor into the work item queue. True and False decides which part of Functor will execute
void ParallelDependencyAnalysis::execute(const std::string & fileSpec)
{
	TaskHelper* pTaskHelper = new TaskHelper(fileSpec, TypeTable,false);
	task.setNewTask((pTaskHelper));

}


