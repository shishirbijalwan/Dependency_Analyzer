/////////////////////////////////////////////////////////////////////
//  QueuedWorkItems.cpp - Thread pool Package                      //
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
#include "QueuedWorkItems.h"
#include "../Utilities/Utilities.h"
//#include "../Project2/Parser/Parser.h"

//----< demonstrate ProcessWorkItem class >----------------------------

using Util = Utilities::StringHelper;
using WorkResult = std::string;
__declspec(dllexport) void getRidOfLNK4221_() {}

#ifdef QUEUEDWORKITEM
int main()
{
  Util::Title("Enqueued Work Items");

  std::cout << "\n  main thread id = " << std::this_thread::get_id();

  ProcessWorkItem<WorkResult> processor;
  //This function starts the dequeu the workItemQueue.. As it does not have any item it will wait for now.
  processor.start();

  // define 1st work item

  WorkItem<WorkResult> wi1 = []() {
    std::cout << "\n  working on thread " << std::this_thread::get_id();
	int i;
	for ( i = 1; i < 5; i++)
		i += i;
	std::stringstream ss;
	ss << i;
	std::string str = ss.str();
    return "wi1 result = " +str;
  };

  processor.doWork(&wi1);

  // define 2nd work item
  
  WorkItem<WorkResult> wi2 = []()
  {
    std::cout << "\n  working on thread " << std::this_thread::get_id();
    size_t sum = 0;
    for (size_t i = 0; i < 100000; ++i)
      sum += i;
    return "wi2 result = " + Utilities::Converter<size_t>::toString(sum);
  };

  processor.doWork(&wi2);
  processor.doWork(&wi2);
  processor.doWork(&wi2);
  processor.doWork(&wi2);



  // the following calls to result() block until results are enqueued

  std::cout << "\n  " << processor.result();
  std::cout << "\n  " << processor.result();
  std::cout << "\n  " << processor.result();
  std::cout << "\n  " << processor.result();
  std::cout << "\n  " << processor.result();
  processor.doWork(nullptr);
  
  // wait for child thread to complete

  processor.wait();
  std::cout << "\n\n";
}
#endif