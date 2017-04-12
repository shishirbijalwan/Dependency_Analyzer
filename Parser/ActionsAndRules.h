#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   CodeParser for CSE687 Pr2, Sp16                 //
//  Author:        Shishir Bijalwan, Syracuse University           //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();               //   and parse it

Build Process:
==============
Required files
-ASTree.h, Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 1.1
Added Rules for Alias and TypeDef
Added Action for Alias and TypeDef
*/
//
#include <queue>
#include <string>
#include <sstream>
#include <map>
#include "Parser.h"
#include "../SemiExp/itokcollection.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "ASTree.h"
#include <algorithm>
//#include "ASTree.cpp"

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.
/*
* ToDo:
* - add AST Node class
* - provide field to hold root of AST
* - provide interface to access AST
* - provide helper functions to build AST, perhaps in Tree class
*/
class Repository  // application specific
{
	Scanner::Toker* p_Toker;
	ASTree* astree;
	std::string	currentDirectory;
	std::map<std::string, std::string> TypeTable;
	std::string currNameSpace;

public:
	Repository(Scanner::Toker* pToker, ASTree* pASTree)
	{	p_Toker = pToker;
		astree = pASTree;		
	}

	Scanner::Toker* Toker()
	{
		return p_Toker;
	}
	ASTree* ASTree() {
		return astree;
	}
	size_t lineCount()
	{
		return (size_t)(p_Toker->currentLineCount());
	}
	void setCurrentDirectory(std::string curr) { currentDirectory = curr; }
	std::string getCurrentDirectory() {	return currentDirectory;}
	void setCurrentNameSpace(std::string nameSpace) { currNameSpace = nameSpace; }
	std::string getCurrentNameSpace() { return currNameSpace; }
	void AddToTypeTable(std::string typePair) {
		std::pair < std::string, std::string> temp(typePair, currentDirectory);
		TypeTable.insert(temp); }
	std::map<std::string, std::string> getTypeTable() { return TypeTable; }
	};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		
		if (pTc->find("{") <pTc->length() && pTc->length()==1)
			{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
	Repository* p_Repos;
public:
	HandlePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
			std::string name = "anonymous";
		std::string type = "unknown";
		size_t lines = p_Repos->lineCount();
		p_Repos->ASTree()->AddNewNode(type, name, lines);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("}") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
	Repository* p_Repos;
public:
	HandlePop(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
	//	p_Repos->ASTree()->endScopeNodeShift(p_Repos->lineCount());


	}
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("#") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
			{
				//std::cout << "Function rule caught" << std::endl;
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
	Repository* p_Repos;
public:
	PushFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{

		std::string name = (*pTc)[pTc->find("(") - 1];
		std::string type = "function";
		size_t lines = p_Repos->lineCount();
		if (pTc->find("[") < pTc->length() && pTc->find("operator")== pTc->length()) {
			type = "Lambda";
				name = "Anonymous";
		}
		p_Repos->ASTree()->AddNewNode(type, name, lines);


	}
};
//Action to Add Functions to type Table
//////////////////////////////////////////////////////////////

class AddToTypeTableFunction : public IAction
{
	Repository* p_Repos;
public:
	AddToTypeTableFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::string temp = p_Repos->ASTree()->getCurrentNodeType();
		//std::cout << "The value of current Node Type is " << temp << std::endl;
		//if (temp != "Class") {
	//	std::cout << "Function action caught" << std::endl;

		if (pTc->find("[")==pTc->length() && pTc->find("[]") == pTc->length()){
				std::string name = (*pTc)[pTc->find("(") - 1];
		
				if(name!="main"){
					name = "Function->" + name;
				p_Repos->AddToTypeTable(name);
		
				}
		}
	}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
	Repository* p_Repos;
public:
	PrintFunction(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  FuncDef: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
	void doAction(ITokCollection*& pTc)
	{
		pTc->remove("public");
		pTc->remove(":");
		pTc->trimFront();
		size_t len = pTc->find(")");
		std::cout << "\n\n  Pretty Stmt:    ";
		for (size_t i = 0; i<len + 1; ++i)
			std::cout << (*pTc)[i] << " ";
		std::cout << "\n";
	}
};

///////////////////////////////////////////////////////////////
// rule to detect Conditional statments and other scopes

class ConditionalStatements : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if"};
		for (int i = 0; i<4; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{

				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to add function name into tree

class PushConditionsalStatements : public IAction
{
	Repository* p_Repos;
public:
	PushConditionsalStatements(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
	
		std::string name = (*pTc)[pTc->find("(") - 1];
		std::string type = "Conditional Statement";
		size_t lines = p_Repos->lineCount();
		p_Repos->ASTree()->AddNewNode(type, name, lines);


		}
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintConditionalStatement : public IAction
{
	Repository* p_Repos;
public:
	PrintConditionalStatement(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  Conditional Statement: " << pTc->show().c_str();
	}
};

///////////////////////////////////////////////////////////////
// rule to detect ClassAndNameSpaceScope statments and other scopes

class ClassAndNameSpace : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "class", "namespace","struct","enum" };
		for (int i = 0; i<4; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}

	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		tc.trimFront();
		if (tc[tc.length() - 1] == "{")
		{
			//std::cout << "Exception here";
			if (isSpecialKeyWord(tc[0]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to add ClassAndNameSpace name into Tree

class PushClassAndNameSpace : public IAction
{
	Repository* p_Repos;
public:
	PushClassAndNameSpace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::string type;;
		std::string name;
		if (pTc->find("class") <pTc->length()) {
			std::string nameSpace = (p_Repos->getCurrentNameSpace());
			nameSpace += "::";
			 name = (*pTc)[pTc->find("class") + 1];
			 name = nameSpace + name;
			 type = "Class";

		}
		else if (pTc->find("struct") < pTc->length()) {
			type = "Struct";
			std::string nameSpace = (p_Repos->getCurrentNameSpace());
			nameSpace += "::";
			name =  (*pTc)[pTc->find("struct") + 1];
			name = nameSpace + name;
		}
		else if (pTc->find("enum") < pTc->length()) {
			type = "enum";
			std::string nameSpace = (p_Repos->getCurrentNameSpace());
			nameSpace += "::";
			name = (*pTc)[pTc->find("enum") + 1];
			name = nameSpace + name;
		}
		else{
			 type = "NameSpace";
			 name = (*pTc)[pTc->find("namespace") + 1];
			 std::string temp = name + "::";
			 p_Repos->setCurrentNameSpace(temp);
		}
		size_t lines = p_Repos->lineCount();
		
		p_Repos->ASTree()->AddNewNode(type, name, lines);
		


		}
};
/// Class to add NameSpace and Class to the Type queue
////////////////////////////////////////////////////////////
class AddTypeClassAndNameSpace : public IAction
{
	Repository* p_Repos;
public:
	AddTypeClassAndNameSpace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::string type;;
		std::string name;
		if (pTc->find("class") <pTc->length()) {
			name = (*pTc)[pTc->find("class") + 1];
			type = "Class->";

		}
		else if (pTc->find("struct") < pTc->length()) {
			type = "Struct->";
			std::string nameSpace = (p_Repos->getCurrentNameSpace());
			name = (*pTc)[pTc->find("struct") + 1];
		}
		else if (pTc->find("enum") < pTc->length()) {
			name = (*pTc)[pTc->find("enum") + 1];
			type = "enum->";
		}
		//if (name.empty()) { std::cout << "This happened in struct namespace area"; };
		if (!(p_Repos->getCurrentNameSpace() == "::")){}
			name = p_Repos->getCurrentNameSpace() + name;

			name = type + name;
		if(pTc->find("namespace") == pTc->length())
		p_Repos->AddToTypeTable(name);
	}
};










///////////////////////////////////////////////////////////////
// action to send ClassAndNameSpace that starts a function def
// to console

class PrintClassAndNameSpace : public IAction
{
	Repository* p_Repos;
public:
	PrintClassAndNameSpace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  Class/Namespace : " << pTc->show().c_str();
	}
};
///////////////////////////////////////////////////////////////
// rule to detect Exception statments and Single word  scopes

class ExceptionRule : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "try", "catch", "do","else" };
		for (int i = 0; i<4; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		tc.trimFront();
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (isSpecialKeyWord(tc[0]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to push Exception Scope and one word scope  into Tree

class PushExceptionScope : public IAction
{
	Repository* p_Repos;
public:
	PushExceptionScope(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::string type;
		std::string name;
		if ((pTc->find("do") <pTc->length())) {
			name = "do";
			type = "Conditional Statement";

		}
		else if (pTc->find("else") < pTc->length()) {
			name = "else";
			type = "Conditional Statement";
		}
		else  {
			type = "try/catch";
			name = (*pTc)[0];
		}
		
		size_t lines = p_Repos->lineCount();

		p_Repos->ASTree()->AddNewNode(type, name, lines);



	}
};
///////////////////////////////////////////////////////////////
// rule to detect declaration scope

class BeginningOfDeclarationScope : public IRule
{
public:

	bool doTest(ITokCollection*& pTc)
	{
		

	if (pTc->find("{") <pTc->length() && pTc->find("=") < (pTc->length() - 1) && (pTc->find("vector") < pTc->length() || pTc->find("[]") < pTc->length()))
{
			
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle creating node for declaraion scope

class DeclarationScopePush : public IAction
{
	Repository* p_Repos;
public:
	DeclarationScopePush(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::string name = "Declaraion scope";
		std::string type = "Declaration";
		size_t lines = p_Repos->lineCount();
		p_Repos->ASTree()->AddNewNode(type, name, lines);
	}
	
};

///////////////////////////////////////////////////////////////
// rule to detect type def

class TypeDefCheck : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "string", "long", "std", "vector","double" };
		for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;


		if (tc.find("typedef") < tc.length())
		{
			//	std::cout << tc.show() << std::endl;
			size_t len = tc.find("typedef");

		//	if (!isSpecialKeyWord(tc[len + 1])) {
			//	std::cout << " Type Def Found " << std::endl;
				doActions(pTc);
				return true;
		//	}
		}
		return true;
	}
};
///////////////////////////////////////////////////////////////
// rule to add using typedef to type table
class AddTypeDefToTypeTable : public IAction
{
	Repository* p_Repos;
public:
	AddTypeDefToTypeTable(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//std::cout << " TypeDef Action " << std::endl;

		std::string name = (*pTc)[pTc->find(";") - 1];
		if(isalpha(name[0])){
		name = "Typedef->" + name;
		p_Repos->AddToTypeTable(name);
		//	std::cout << " TypeDef Added " << std::endl;

		}

	}
};


///////////////////////////////////////////////////////////////
// rule to detect using alias

class AliasCheck : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "string", "vector", "int", "char" };
		for (int i = 0; i<4; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		

		if (tc.find("using") < tc.length() && tc.find("namespace")== tc.length())
		{
		//	std::cout << tc.show() << std::endl;
			size_t len = tc.find(";");

		//	if (!isSpecialKeyWord(tc[len-1])){			
				
				doActions(pTc);
				return true;
			//}
		}
		return true;
	}
};
///////////////////////////////////////////////////////////////
// rule to add using alias to type table
class AddAliasToTypeTable : public IAction
{
	Repository* p_Repos;
public:
	AddAliasToTypeTable(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		
			std::string name = (*pTc)[pTc->find("using") + 1];

				name = "Alias->" + name;
				p_Repos->AddToTypeTable(name);
					
		
	}
};



#endif
