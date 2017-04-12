#ifndef IFILEMGR_H
#define IFILEMGR_H
/////////////////////////////////////////////////////////////////////
//  IFileMgr.h - interfaces for handling events                    //
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
This package is to help the File Manager package to create the object factory and the define the 
struct which can be inherited to do various activities in case of a event like a file is found
the IFileEventHandler struct and in case of a change in directory IDirEventHandler struct.  We will
use these structs in order to do parallel processing for dependency and type anaylysis.

Public Interface:
=================
execute							// This is execute a method in case a file is found
setNullToCloseThread			// This is to close the threads
waitProcessor					// This method call the thead join condition
startThread						// This method is to start the threads
addPattern						// This is to add the patterns to File Manager
search							// To start the file search process
regForFiles						// For adding object pointer for file event handling
regForDirs						// This to  add object pointer for directory event handling

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
namespace FileManager
{
  struct IFileEventHandler
  {
    virtual void execute(const std::string& fileSpec) = 0;
	virtual void setNullToCloseThread() = 0;
	virtual void waitProcessor() = 0;
	virtual void startThread() = 0;
  };

  struct IDirEventHandler
  {
    virtual void execute(const std::string& dirSpec) = 0;
  };

  struct IFileMgr
  {
    virtual ~IFileMgr() {}
    virtual void addPattern(const std::string& patt) = 0;
    virtual void search() = 0;
    virtual void find(const std::string& path) = 0;
    virtual void regForFiles(IFileEventHandler* pEventHandler) = 0;
    virtual void regForDirs(IDirEventHandler* pEventHandler) = 0;
  };

  struct FileMgrFactory
  {
    static IFileMgr* create(const std::string& path);
  };
}
#endif