/////////////////////////////////////////////////////////////////////
//  Task_Helper.cpp - Helper class for workitems                     //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   Parallel Dependency Analysis, Sp16               //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////
// This does not have test stub as we have tested this in Task.cpp

#include "TaskHelper.h"
#include <iostream>
#include "../Parser/Parser.h"
#include "../Parser/ConfigureParser.h"
#include "../DependencyAnalysis/DependencyAnalysis.h"

// Below we have created functor which executes method based on the flag condition which is sent at the time of object creation
std::map<std::string, std::string> TaskHelper::operator()()
{		
if(TaskHelperFlag){
	// for type Analysis
	ConfigParseToConsole configure;
		Parser* pParser;
		pParser = configure.Build();
	configure.Attach(fileName);
	configure.setCurrentDirectory(fileName);
	while (pParser->next())
		pParser->parse();
	tempTypeTable = configure.getTypeTable();
		
	return tempTypeTable;
}
else {
	//for dependency analaysis
	DependencyAnalysis dependencyAnalysis(Phase1TypeTable, fileName);
	dependencyAnalysis.findDependecy();
	TempDependencyTable = dependencyAnalysis.gettempDependencyTable();
	return TempDependencyTable;
}

}
//This is constructor which is to intialize various  member variables
TaskHelper::TaskHelper(std::string file, std::unordered_multimap<std::string, std::string> table, bool flag) { 
	fileName = file;
	Phase1TypeTable = table;
	TaskHelperFlag = flag;
};
