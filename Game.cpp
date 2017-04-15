
#include "Policy.h"
#include "HuntingPolicy.h"
#include "SensingPolicy.h"
#include "Prey.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>

#define TARGET_GAMES 500000

// Game Values
int _map[MAP_WIDTH][MAP_HEIGHT];
int _gamesPlayed, _totalReward;
int rewards[50];
int rewards_end[5];
int messages_end[5][4];
int _width = MAP_WIDTH, _height = MAP_HEIGHT;

// Agents
HuntingPolicy _hunter;
HuntingPolicy _sensor;
SensingPolicy _sensorvision;
Prey prey;

enum Objects
{
	UNothing,
	UAgent,
	UGoal
};

#define END_EPS 5
#define BEGIN_EPS 25

void init_map()
{
	_map[_hunter._posX][_hunter._posY] = UNothing;
	_map[_sensor._posX][_sensor._posY] = UNothing;
	_map[prey._posX][prey._posY] = UNothing;
	_map[1][1] = UAgent;
	_map[MAP_WIDTH/2][MAP_HEIGHT/2] = UGoal;
	_map[MAP_WIDTH-1][MAP_HEIGHT-1] = UAgent;
	_hunter.SendObservation(OSelfX,1);
	_hunter.SendObservation(OSelfY,1);
	_hunter.SendObservation(OPreyX,MAP_WIDTH);
	_hunter.SendObservation(OPreyY,MAP_HEIGHT);
	_sensor.SendObservation(OSelfX,MAP_WIDTH-1);
	_sensor.SendObservation(OSelfY,MAP_HEIGHT-1);
	prey.SendObservation(OSelfX,MAP_WIDTH/2);
	prey.SendObservation(OSelfY,MAP_HEIGHT/2);
#if SIMPLE_COMM == true
	_sensorvision.SendObservation(OOtherX,_hunter._posX);
	_sensorvision.SendObservation(OOtherY,_hunter._posY);
	_sensorvision.SendObservation(OPreyX,prey._posX);
	_sensorvision.SendObservation(OPreyY,prey._posY);
#else
	_sensorvision.SendObservation(OPreyX,2);
	_sensorvision.SendObservation(OPreyY,2);
#endif
}

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
	_map[MAP_WIDTH/2][MAP_HEIGHT/2] = UGoal;
	_map[MAP_WIDTH-1][MAP_HEIGHT-1] = UAgent;
	

	_hunter.eps = BEGIN_EPS;
	_sensor.eps = BEGIN_EPS;
	_sensorvision.eps = BEGIN_EPS;
	
	_hunter.SendObservation(OSelfX,1);
	_hunter.SendObservation(OSelfY,1);
	_sensor.SendObservation(OSelfX,9);
	_sensor.SendObservation(OSelfY,9);
	_sensor.SendObservation(OOtherX, MAP_WIDTH);
	_sensor.SendObservation(OOtherY, MAP_HEIGHT);
	_sensor.SendObservation(OOtherX, MAP_WIDTH);
	_sensor.SendObservation(OOtherY, MAP_HEIGHT);
	_sensor.SendObservation(OPreyX, MAP_WIDTH);
	_sensor.SendObservation(OPreyY, MAP_HEIGHT);
	_sensor.SendObservation(OPreyX, MAP_WIDTH);
	_sensor.SendObservation(OPreyY, MAP_HEIGHT);
	Policy* l[2];
	l[0] = &_hunter;
	l[1] = &_sensor;
	prey = Prey(l);
	prey.SendObservation(OSelfX,5);
	prey.SendObservation(OSelfY,5);
	
}

void init_extra(int w, int h)
{
	_gamesPlayed = 0;
	_totalReward = 0;
	_width = w;
	_height = h;

	for(int i = 0; i < w; i++) {
		for(int j = 0; j < h; j++) {
			_map[i][j] = UNothing;
		}	
	}
	
	_map[1][1] = UAgent;
	_map[w/2][h/2] = UGoal;
	_map[w-1][h-1] = UAgent;
	_sensor.isSensor = true;

	
	_hunter.eps = BEGIN_EPS;
	_sensor.eps = BEGIN_EPS;
	_sensorvision.eps = BEGIN_EPS;
	
	_hunter.SendObservation(OSelfX,1);
	_hunter.SendObservation(OSelfY,1);
	_sensor.SendObservation(OSelfX,w-1);
	_sensor.SendObservation(OSelfY,h-1);
	_sensor.SendObservation(OOtherX, w);
	_sensor.SendObservation(OOtherY, h);
	_sensor.SendObservation(OOtherX, w);
	_sensor.SendObservation(OOtherY, h);
	_sensor.SendObservation(OPreyX, w);
	_sensor.SendObservation(OPreyY, h);
	_sensor.SendObservation(OPreyX, w);
	_sensor.SendObservation(OPreyY, h);
	Policy* l[2];
	l[0] = &_hunter;
	l[1] = &_sensor;
	prey = Prey(l);
	prey.SendObservation(OSelfX,w/2);
	prey.SendObservation(OSelfY,h/2);
	
}

void moveagent(Policy& agent, int newX, int newY)
{
	// Store Temps
	int preX = agent._posX;
	int preY = agent._posY;
	
	// Move Agent
	agent.SendObservation(OSelfX,newX);
	agent.SendObservation(OSelfY,newY);
	
	// Swap Tiles
	int preV = _map[preX][preY];
	_map[preX][preY] = _map[newX][newY];
	_map[newX][newY] = preV;
	
}

void getcheckvalues(int& xcheck, int& ycheck, int action)
{
	if(action == AUp) {
		ycheck -= 1;
	} else if(action == ADown) {
		ycheck += 1;
	} else if(action == ALeft) {
		xcheck -= 1;
	} else if(action == ARight) {
		xcheck += 1;
	}
}

#define DISPLAY_MOVES false
#define TELL_PREY true
#define PERFORM_COMM false
#define TELL_OTHER false
#define SPLIT_REWARD true
#define BOTH_HUNTERS false

bool debug_print = false;

int nullmessages = 0, xmessages = 0, ymessages = 0, xymessages = 0;

void performcommunication()
{
#if SIMPLE_COMM == true
	//_sensorvision.SendObservation(OPreyX, prey._posX);
	//_sensorvision.SendObservation(OPreyY, prey._posY);
	//_sensorvision.SendObservation(OOtherX, _hunter._posX);
	//_sensorvision.SendObservation(OOtherY, _hunter._posY);
	_sensorvision.SendObservation(OPreyX, 0);
	_sensorvision.SendObservation(OPreyY, 0);
	_sensorvision.SendObservation(OOtherX, 0);
	_sensorvision.SendObservation(OOtherY, 0);	
#else
	int xabs = (SENSING_WIDTH - 1)/2;
	int yabs = (SENSING_HEIGHT - 1)/2;
	
	int relx = _sensor._posX - xabs;
	int rely = _sensor._posY - yabs;
	
	relx = prey._posX - relx;
	rely = prey._posY - rely;
	
	if(relx >= 0 && relx < SENSING_WIDTH && rely >= 0 && rely < SENSING_HEIGHT) {
		_sensorvision.SendObservation(OPreyX, relx);
		_sensorvision.SendObservation(OPreyY, rely);
	}
	else {
		_sensorvision.SendObservation(OPreyX, xabs+1);
		_sensorvision.SendObservation(OPreyY, yabs+1);		
	}
#endif	
	
	
	int action = _sensorvision.GetAction();
	switch (action) {
		case CPreyX:
			xmessages++;
			_hunter.SendObservation(OPreyX,prey._posX);
			_sensorvision.SendObservation(OReward, -2);
			if(_gamesPlayed >= TARGET_GAMES-4) messages_end[4-(TARGET_GAMES-_gamesPlayed)][1]++;
			break;
		case CPreyY:
			ymessages++;
			_hunter.SendObservation(OPreyY,prey._posY);
			_sensorvision.SendObservation(OReward, -2);
			if(_gamesPlayed >= TARGET_GAMES-4) messages_end[4-(TARGET_GAMES-_gamesPlayed)][2]++;			
			break;
		case CPreyXY:
			xymessages++;
			_hunter.SendObservation(OPreyX,prey._posX);
			_hunter.SendObservation(OPreyY,prey._posY);
			_sensorvision.SendObservation(OReward, -3);
			if(_gamesPlayed >= TARGET_GAMES-4) messages_end[4-(TARGET_GAMES-_gamesPlayed)][3]++;			
			break;
		default:
			nullmessages++;
			_sensorvision.SendObservation(OReward,-1);
			if(_gamesPlayed >= TARGET_GAMES-4) messages_end[4-(TARGET_GAMES-_gamesPlayed)][0]++;			
			break;
	}
		if(debug_print) {
		std::cout << "======" << std::endl;
		std::cout << "Vision:" << _sensor._posX << ":" << _sensor._posY << std::endl;
		std::cout << "Vision Action:" << action << std::endl;
	}
	
}
int performactions(int& loops, bool& done)
{
	int action = prey.GetAction();
	
	int xToCheck = prey._posX, yToCheck = prey._posY;
	
	if(debug_print) {
		std::cout << std::endl << "======" << std::endl;
		std::cout << "Prey:" << prey._posX << ":" << prey._posY << std::endl;
		std::cout << "Prey Action:" << action << std::endl;
	}		
	getcheckvalues(xToCheck, yToCheck, action);
	
	
	if(yToCheck < 0 || yToCheck >= _height || xToCheck < 0 || xToCheck >= _width) {
		moveagent(prey, prey._posX, prey._posY);
	} else if(_map[xToCheck][yToCheck] == UNothing) {
		moveagent(prey, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UAgent) {
		moveagent(prey, prey._posX, prey._posY);
	} else if(_map[xToCheck][yToCheck] == UGoal) {
	}
	if(debug_print) {
		std::cout << "Prey After:" << prey._posX << ":" << prey._posY << std::endl;
	}
#if TELL_PREY == true
	_hunter.SendObservation(OPreyX, prey._posX);
	_hunter.SendObservation(OPreyY, prey._posY);
#else
	_hunter.SendObservation(OPreyX, MAP_WIDTH);
	_hunter.SendObservation(OPreyY, MAP_HEIGHT);	
#endif
	_sensor.SendObservation(OPreyX, prey._posX);
	_sensor.SendObservation(OPreyY, prey._posY);
#if PERFORM_COMM == true
	performcommunication();
#endif
#if TELL_OTHER == true
	_sensor.SendObservation(OOtherX, _hunter._posX);
	_sensor.SendObservation(OOtherY, _hunter._posY);
	_hunter.SendObservation(OOtherX, _sensor._posX);
	_hunter.SendObservation(OOtherY, _sensor._posY);
#else
	_sensor.SendObservation(OOtherX, _width);
	_sensor.SendObservation(OOtherY, _height);
	_hunter.SendObservation(OOtherX, _width);
	_hunter.SendObservation(OOtherY, _height);
#endif

	
	action = _hunter.GetAction();
	int reward = 0;
	xToCheck = _hunter._posX, yToCheck = _hunter._posY;
	
	getcheckvalues(xToCheck, yToCheck, action);
	if(debug_print) {
		std::cout << "======" << std::endl;
		std::cout << "Hunter:" << _hunter._posX << ":" << _hunter._posY << std::endl;
		std::cout << "HunterPrey:" << _hunter._preyX << ":" << _hunter._preyY << std::endl;
		std::cout << "HunterOther:" << _hunter._otherX << ":" << _hunter._otherY << std::endl;
		std::cout << "Hunter Action:" << action << std::endl;
	}
	
	if(yToCheck < 0 || yToCheck >= _height || xToCheck < 0 || xToCheck >= _width) {
		reward -= 100;
		_totalReward += reward;
		moveagent(_hunter, _hunter._posX, _hunter._posY);
	} else if(_map[xToCheck][yToCheck] == UNothing) {
		reward -= 10;
		//reward -= abs(_hunter._posX - prey._posX)*10 + abs(_hunter._posY - prey._posY)*10;
		_totalReward += reward;
		moveagent(_hunter, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UAgent) {
		reward -= 50;
		_totalReward += reward;
		moveagent(_hunter, _hunter._posX, _hunter._posY);
	} else if( _map[xToCheck][yToCheck] == UGoal) {//_map[xToCheck][yToCheck] == UGoal) {xToCheck == prey._posX && yToCheck == prey._posY 
		reward += GOAL_REWARD;
		_totalReward += reward;
		//_hunter.SendObservation(OReward, reward);
		init_map();
		// moveagent(_hunter, 1, 1);
		// moveagent(prey,_width/2,_height/2);
		// moveagent(_sensor, _width-1, _height-1);
		// _sensorvision.SendObservation(OPreyX,2);
		// _sensorvision.SendObservation(OPreyY,2);
		_sensorvision.SendObservation(OReward,reward);
		loops = 0;
	}
	
	if(debug_print) {
		std::cout << "Hunter After:" << _hunter._posX << ":" << _hunter._posY << std::endl;
	}
	
#if SPLIT_REWARD == true
	_hunter.SendObservation(OReward, reward);
	reward = 0;
#endif


	action = _sensor.GetAction();
	xToCheck = _sensor._posX, yToCheck = _sensor._posY;
	
	getcheckvalues(xToCheck, yToCheck, action);
	if(debug_print) {
		std::cout << "======" << std::endl;
		std::cout << "Sensor:" << _sensor._posX << ":" << _sensor._posY << std::endl;
		std::cout << "SensorPrey:" << _sensor._preyX << ":" << _sensor._preyY << std::endl;
		std::cout << "SensorOther:" << _sensor._otherX << ":" << _sensor._otherY << std::endl;
		std::cout << "Sensor Action:" << action << std::endl;
	}
	
	if(yToCheck < 0 || yToCheck >= _height || xToCheck < 0 || xToCheck >= _width) {
		reward -= 100;
		_totalReward += reward;
		moveagent(_sensor, _sensor._posX, _sensor._posY);
	} else if(_map[xToCheck][yToCheck] == UNothing) {
		reward -= 10;
		//reward -= abs(_sensor._posX - prey._posX)*10 + abs(_sensor._posY - prey._posY)*10;
		_totalReward += reward;
		moveagent(_sensor, xToCheck, yToCheck);
	} else if(_map[xToCheck][yToCheck] == UAgent) {
		reward -= 50;
		_totalReward += reward;
		moveagent(_sensor, _sensor._posX, _sensor._posY);
	} else if(_map[xToCheck][yToCheck] == UGoal) {
		#if BOTH_HUNTERS == false
		reward -= 50;
		_totalReward += reward;
		moveagent(_sensor, _sensor._posX, _sensor._posY);
		#else
		reward += GOAL_REWARD;
		_totalReward += reward;
		moveagent(_hunter, 1, 1);
		moveagent(prey,_width/2,_height/2);
		moveagent(_sensor, _width-1, _height-1);
		_sensorvision.SendObservation(OPreyX,2);
		_sensorvision.SendObservation(OPreyY,2);
		_sensorvision.SendObservation(OReward,reward);		
		loops = 0;
		#endif
	}
	
	if(debug_print) {
		std::cout << "Sensor After:" << _sensor._posX << ":" << _sensor._posY << std::endl;
	}
	
#if SPLIT_REWARD == true
	_sensor.SendObservation(OReward, reward);
#else
	_hunter.SendObservation(OReward, reward);
	_sensor.SendObservation(OReward, reward);
#endif
	

	if(loops == 0) {
		if(_totalReward > GOAL_REWARD*2) _totalReward = -1e9;
		if(_gamesPlayed % 10000 == 0) std::cout << _totalReward << " Reward for Game #" << _gamesPlayed << std::endl;
		if(_gamesPlayed == TARGET_GAMES) done = true;
		if(_gamesPlayed > TARGET_GAMES/2) {
			_sensorvision.eps = END_EPS;
			_hunter.eps = END_EPS;
			_sensor.eps = END_EPS;
		}
		if(_gamesPlayed < 50) rewards[_gamesPlayed] = _totalReward;
		if(_gamesPlayed >= TARGET_GAMES-4) rewards_end[4-(TARGET_GAMES-_gamesPlayed)] = _totalReward;
		_gamesPlayed++;
		_totalReward = 0;
	}
	
	return reward;
}




void playgame()
{
	
	// Call moveagent at least once per agent.
	// If no movement required, call for same location.
	
	bool done = false;
	int loops = 0, limit = 1e9;
	clock_t begin = clock();
	while(!done && loops < limit) 
	{
		loops++;
		if(float(clock() - begin) / CLOCKS_PER_SEC > 10) {
			begin = clock();
			std::cout << "Games Played so far: " << _gamesPlayed << std::endl;
		}
		
		performactions(loops,done);
		//performcommunication();
		if(debug_print) {
			std::cout << "======" << std::endl;
			std::cout << "MAP" << std::endl;
			for(int i = 0; i < MAP_WIDTH; i++) {
				for(int j = 0; j < MAP_HEIGHT; j++) {
					std::cout << _map[j][i];
				}
				std::cout << std::endl;
			}
		std::cout << "======" << std::endl << std::endl;
		}
		if(loops == 0) begin = clock();
		
		if(!(loops < limit)) std::cout << "Loop Limit" << std::endl;
	}
}

int main(int argc, char* args[] )
{ 
	try{
		srand(time(NULL));
		init_extra(4,4);
		playgame();
		
		std::cout << "Rewards for 5 first games" << std::endl;
		std::cout << "------===========------" << std::endl;
		for(int i = 0; i < 5; i++) {
			std::cout << "Game #" << i+1 << ": " << rewards[i] << std::endl;
		}
		std::cout << "Rewards for 5 last games" << std::endl;
		std::cout << "------===========------" << std::endl;
		for(int i = 0; i < 5; i++) {
			std::cout << "Game #" << i+1 << ": " << rewards_end[i] << std::endl;
			std::cout << "Messages for #" << i+1 << std::endl;
			std::cout << "\tNull Messages: \t\t" << messages_end[i][0] << std::endl;
			std::cout << "\tX Only Messages: \t" << messages_end[i][1] << std::endl;
			std::cout << "\tY Only Messages: \t" << messages_end[i][2] << std::endl;
			std::cout << "\tXY Messages: \t\t" << messages_end[i][3] << std::endl;			
		}
		std::cout << "Total Messages" << std::endl;		
		std::cout << "------===========------" << std::endl;
		std::cout << "Null Messages: \t\t" << nullmessages << std::endl;
		std::cout << "X Only Messages: \t" << xmessages << std::endl;
		std::cout << "Y Only Messages: \t" << ymessages << std::endl;
		std::cout << "XY Messages: \t\t" << xymessages << std::endl;
		_sensorvision.printpolicy();
		//for(int i = 0; i < 100; i++) {
		//	std::cout << "RNG #" << i << ": " << _sensorvision.rngness[i] << std::endl;
		//}*/
		
		//std::cout << "Policy Map for Hunter" << std::endl;
		//_hunter.PrintPolicy();
	}
	catch (...) { 
		std::cout << "Exception Occured" << std::endl;
	}
	
	return 0;
}