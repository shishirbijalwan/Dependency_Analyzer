/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                  //
//  ver 1.4                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp09                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace Utilities;

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
    for(size_t i=0; i<actions.size(); ++i)
      actions[i]->doAction(pTokColl);
}

//----< test stub >--------------------------------------------

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

#ifdef TEST_PARSER

#include <queue>
#include <string>
#define Util StringHelper

int main(int argc, char* argv[])
{


    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
	
 
	configure.Attach("C:\\Users\\Megha\\Desktop\\RealProject_3\\Try1\\Project2\\Test_Files\\Req6\\Shish.txt");
	configure.setCurrentDirectory("C:\\Users\\Megha\\Desktop\\RealProject_3\\Try1\\Project2\\Test_Files\\Req6\\Shish.txt");
	while(pParser->next())
        pParser->parse();

	  std::cout << "Parser Execution went fine" << std::endl;
      std::cout << "\n";
  
	  std::cout << "============================Values in Type Table======================================" << std::endl;
	  std::map<std::string, std::string> TypeTable = configure.getTypeTable();
	  for (std::map<std::string, std::string>::iterator it = TypeTable.begin(); it != TypeTable.end(); it++) {

		  std::cout << it->first << "     " << it->second << std::endl;
	  }
	  std::cout << "======================================================================================" << std::endl;
  
  std::cout << "\n";
}

#endif
