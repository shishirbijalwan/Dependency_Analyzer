#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////
// FileMgr.h - find files matching specified patterns              //
//             on a specified path                                 //
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
The file manager pacakage works on our file system. It takes the directory path where it has to
act as the argument in the object factory. This directory acts as the root directory. The file
manager then with the search method find each and every file based on the pattern given. In case
no pattern is given it gives all the files one by one.

Public Interface:
=================
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
#include "IFileMgr.h"
#include "FileSystem.h"
#include <iostream>
#include <chrono>

namespace FileManager
{
  class FileMgr : public IFileMgr
  {
  public:
    using patterns = std::vector<std::string>;
    using fileSubscribers = std::vector<IFileEventHandler*>;
    using dirSubscribers = std::vector<IDirEventHandler*>;

    FileMgr(const std::string& path) : path_(path)
    {
      patterns_.push_back("*.*");
    }
// This method is to add the different patterns of file we want to search 
    void addPattern(const std::string& patt)
    {
      if (patterns_.size() == 1 && patterns_[0] == "*.*")
        patterns_.pop_back();
      patterns_.push_back(patt);
    }
// This method starts the thread pool and then starts the file search. After that it waits till all threads complete the work
    void search()
    {
		fileSubscribers_[0]->startThread();
      find(path_);
	  fileSubscribers_[0]->setNullToCloseThread();
	  fileSubscribers_[0]->waitProcessor();
    }
// This method help the the search method in finding file and calling executive methods based on the event
    void find(const std::string& path)
    {
      std::string fpath = FileSystem::Path::getFullFileSpec(path);
      for (auto pEvtHandler : dirSubscribers_)
      {
        pEvtHandler->execute(fpath);
      }
      for (auto patt : patterns_)
      {
        std::vector<std::string> files = FileSystem::Directory::getFiles(fpath, patt);
        for (auto f : files)
        {
          for (auto pEvtHandler : fileSubscribers_)
          {
			  f = FileSystem::Path::getFullFileSpec(fpath) + "\\" + f;
            pEvtHandler->execute(f);
          }
        }
      }
      std::vector<std::string> dirs = FileSystem::Directory::getDirectories(fpath);
      for (auto d : dirs)
      {
        if (d == "." || d == "..")
          continue;
        std::string dpath = fpath + "\\" + d;
        find(dpath);
      }
    }
// This is to add object which will handle file find event
    void regForFiles(IFileEventHandler* pHandler)
    {
      fileSubscribers_.push_back(pHandler);
    }
// This is to add object which will handle directory find event
    void regForDirs(IDirEventHandler* pHandler)
    {
      dirSubscribers_.push_back(pHandler);
    }
  private:
    std::string path_;
    patterns patterns_;
    fileSubscribers fileSubscribers_;
    dirSubscribers dirSubscribers_;
  };
  // This is object factory
  inline IFileMgr* FileMgrFactory::create(const std::string& path)
  {
    return new FileMgr(path);
  }
}
#endif
