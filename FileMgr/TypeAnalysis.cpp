/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.cpp-Type analysis package                         //
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
#include "../FileMgr/TypeAnalysis.h"
#include "../Parser/Parser.h"
#include "../QueuedWorkItems/TaskHelper.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
#include "FileSystem.h"
#include <unordered_map>

#ifdef TEST_TYPEANALYSIS



int main(int argc, char* argv[])
{
	TypeAnalysis* pal = new TypeAnalysis();
	std::string path = "C:\\Users\\Megha\\Desktop\\RealProject_3\\Try1\\Project2\\Test_Files\\Test_Executive\\Tokenizer\\";

	FileManager::IFileMgr* pFmgr = FileManager::FileMgrFactory::create(path);
    pFmgr->regForFiles(pal);
    pFmgr->search();

	std::unordered_multimap<std::string, std::string> Result;
	Result = pal->getResultTable();


	for (std::unordered_multimap<std::string, std::string>::iterator it = Result.begin(); it != Result.end(); it++) {

		std::cout << "Type " << it->first << " In File  " << it->second << std::endl;
		//std::cout << "File- " << it->second << std::endl;
		//std::cout << "   dependent on- " << it->first << std::endl;


	}

	return 0;
}

#endif




// This execute method is executed every time the file manager get a file.
//This method created the object of Task_Helper class and enqueue in workitem.
void TypeAnalysis::execute(const std::string & fileSpec)
{
	std::unordered_multimap<std::string, std::string> map1;
	TaskHelper* pTaskHelper = new TaskHelper(fileSpec, map1, true);
	task.setNewTask((pTaskHelper));
}
