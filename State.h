#ifndef __STATE_H__
#define __STATE_H__

#include <map>
#include <string>
#include <vector>

class State
{
	private:
		std::map<std::string,int> _stateMap;
	
	public:
		void SetVariable(std::string,int);
};

#endif

