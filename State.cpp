
#include "State.h"
// State

void State::SetVariable(std::string variable_name, int value)
{
	_stateMap[variable_name]=value;
}

void State::RegisterVariable(std::string variable_name)
{
	_variableList.push_back(variable_name);
}