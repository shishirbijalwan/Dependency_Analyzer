#include "TaskHelper.h"

std::map<std::string, std::string> TaskHelper::operator()()
{
	std::cout << "Functor called" << std::endl;
	return tempTypeTable;
}
