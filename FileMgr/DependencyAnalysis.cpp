/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.cpp - Analyzes C++ language constructs         //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015, SP1                            //
//  Platform:      HP Pallvilion, Windows 7                        //
//  Application:   CodeParser for CSE687 Pr2, Sp16                 //
//  Author:        Shishir Bijalwan, Syracuse University           //
//                 (979) 587-6340, sbijalwa@syr.edu                //
/////////////////////////////////////////////////////////////////////


#include <iostream>
#include <string>
#include <vector>
#include "../Tokenizer/Tokenizer.h"

#include "DependencyAnalysis.h"
using namespace Scanner;
#ifdef TEST_DEPENDENCYANALYSIS



int main(int argc, char* argv[])
{
	TypeAnalysis type;
	type.search();
	return 0;
}

#endif

DependencyAnalysis::DependencyAnalysis(std::map<std::string, std::string> tmap)
{
	typeTable = tmap;
}

void DependencyAnalysis::getDependencyAnalysis(std::string file)
{
	std::ifstream in(file);
	Toker toker;
	toker.returnComments();
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		for (std::map<std::string, std::string>::iterator it = typeTable.begin(); it != typeTable.end(); it++) {
		std::string dependent=	it->first;
		std::size_t found = dependent.find(tok);
		if (found != std::string::npos) {
			std::pair<std::string, std::string> pairOfDependent(dependent, file);
			DependencyTable.insert(pairOfDependent);

		}
			
		}
		
	} while (in.good());

}
