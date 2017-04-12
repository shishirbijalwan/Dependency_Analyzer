/////////////////////////////////////////////////////////////////////
//  Task.cpp - Wrapper package for Thread pool                     //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   Parallel Dependency Analysis, Sp16               //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////

#include "Task.h"
#include <string>
#include <iostream>
#include <conio.h>
#include "QueuedWorkItems.h"
#include "../Utilities/Utilities.h"


using Util = Utilities::StringHelper;
using WorkResult =std:: map<std::string, std::string>;

#ifdef TEST_TASK
int main()
{
Task task;

std::cout << "The Main Part Started" << std::endl;
	
std::string tempstr = "This is Display Check";
std::map<std::string, std::string> map1;
bool flag = true;
TaskHelper* taskHelper= new TaskHelper(tempstr, map1, flag);
task.setNewTask(taskHelper);
TaskHelper* taskHelper2 = new TaskHelper(tempstr, map1, false);
task.setNewTask(taskHelper2);
task.startThread();
task.setNullToCloseThread();
task.waitProcessor();


	
	return 0;
}

#endif
// This method is to add the new work item and add the workitem count which helps in result retirval
void Task::setNewTask(TaskHelper*  pWi)
{
	processor.doWork(pWi);
	taskCount = taskCount + 1;
}

// This function merges the results from different thread into one and return the Final result
std::unordered_multimap<std::string, std::string> Task::getResult()
{
	int val = taskCount;
	for (int i = 0; i < val; i++) {
		std::map<std::string, std::string>  temp1;
		temp1 = processor.result();
		FinalResult.insert(temp1.begin(), temp1.end());
			}
	return FinalResult;
}
// This function is for setting the nullptr in the workitem queue based on the number of thread generated to shut them down.
void Task::setNullToCloseThread()
{
	int max = processor.getmax_Threads();
	for (int i = 0; i < max; i++) {
		processor.doWork(nullptr);

	}

}
// This is a method to merge the result for final dependency table
std::unordered_map<std::string, std::string> Task::ResultForDependencyAnalysis()
{
	int val = taskCount;
	for (int i = 0; i < val; i++) {
		//	std::cout << "Inside value transfer" << std::endl;
		std::map<std::string, std::string>  temp1;
		temp1 = processor.result();
		FinalForDependencyTable.insert(temp1.begin(), temp1.end());
	}
	return FinalForDependencyTable;
}
