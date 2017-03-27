
#include "Policy.h"
#include "HuntingPolicy.h"
#include <iostream>

// Game Values
int _map[MAP_WIDTH][MAP_HEIGHT];
int _gamesPlayed, _totalReward;
int rewards[50];

// Agents
HuntingPolicy _hunter;

enum Objects
{
	UNothing,
	UAgent,
	UGoal
};

void init()
{
	_gamesPlayed = 0;
	_totalReward = 0;

	for(int i = 0; i < MAP_WIDTH; i++) {
		for(int j = 0; j < MAP_HEIGHT; j++) {
			_map[i][j] = UNothing;
		}	
	}
	
	_map[1][1] = UAgent;
	_map[9][9] = UGoal;
	
	_hunter.SendObservation(OSelfX,1);
	_hunter.SendObservation(OSelfY,1);

}

void moveagent(Policy& agent, int newX, int newY)
{
	// Store Temps
	int prex = agent._posX;
	int prey = agent._posY;
	
	// Move Agent
	_hunter.SendObservation(OSelfX,newX);
	_hunter.SendObservation(OSelfY,newY);
	
	// Swap Tiles
	int prev = _map[prex][prey];
	_map[prex][prey] = _map[newX][newY];
	_map[newX][newY] = prev;
	
}

void playgame()
{
	
	// Call moveagent at least once per agent.
	// If no movement required, call for same location.
	
	bool done = false;
	int loops = 0;
	while(!done && loops < 10000) 
	{
		int action = _hunter.GetAction();
		int reward = 0;
		int xToCheck = _hunter._posX, yToCheck = _hunter._posY;
		
		if(action == AUp) {
			yToCheck = _hunter._posY - 1;
		} else if(action == ADown) {
			yToCheck = _hunter._posY + 1;
		} else if(action == ALeft) {
			xToCheck = _hunter._posX - 1;
		} else if(action == ARight) {
			xToCheck = _hunter._posX + 1;
		}
		
		if(yToCheck < 0 || yToCheck >= MAP_HEIGHT || xToCheck < 0 || xToCheck >= MAP_WIDTH) {
			reward -= 100;
			_totalReward += reward;
			moveagent(_hunter, _hunter._posX, _hunter._posY);
		} else if(_map[xToCheck][yToCheck] == UNothing) {
			reward -= 10;
			_totalReward += reward;
			moveagent(_hunter, xToCheck, yToCheck);
		} else if(_map[xToCheck][yToCheck] == UAgent) {
			reward -= 50;
			_totalReward += reward;
			moveagent(_hunter, xToCheck, yToCheck);
		} else if(_map[xToCheck][yToCheck] == UGoal) {
			reward += 500;
			_totalReward += reward;
			
			moveagent(_hunter, 1, 1);
			if(_gamesPlayed % 100 == 0) std::cout << _totalReward << " Reward for Game #" << _gamesPlayed << std::endl;
			if(_gamesPlayed == 5000) done = true;
			
			
			if(_gamesPlayed < 50) rewards[_gamesPlayed] = _totalReward;
			_gamesPlayed++;
			_totalReward = 0;
		}
		
		// Send Rewards
		_hunter.SendReward(reward);
	}
}

int main(int argc, char* args[] )
{ 
	try{
		init();
		playgame();
		
		std::cout << "Rewards for 50 first games" << std::endl;
		std::cout << "------===========------" << std::endl;
		for(int i = 0; i < 50; i++) {
			std::cout << "Game #" << i << ": " << rewards[i] << std::endl;
		}
	}
	catch (...) { 
		std::cout << "Exception Occured" << std::endl;
	}
	
	return 0;
}