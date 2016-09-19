#pragma once
#include <vector>
#include <queue>
#include <cassert>
#include <time.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{
private:
	int score;
public:
	Player() { score = 0; }
	void addScore(int val) { score += val; }
	int getScore() { return score; }
};

struct Box
{
	enum State { occupied, wall, empty } state;
	int score;
	int playerIndex = -1;
};

class Game
{
private:
	std::vector<std::vector<Box>> boxArray;
	std::vector<std::queue<int>> playerscore;
	std::vector<Player> playerList;
	int currentPlayer;
	int available_Box;

	void helper_randomize(std::vector<int>& vector);	//randomize the scoreList
public:
	Game(int num_player = 4, int MAP_WIDTH = 12, int MAP_HEIGHT = 12, int MAP_NUM_WALL = 20);
	int getScore(const int& playerIndex);	//get that player's score
	int getCurrentPlayer() { return currentPlayer; }	//get the currentPlayer
	int getNumPlayer() { return playerList.size(); }	//get number of players
	std::queue<int> const* getScoreQueue(int playerIndex) ;	//get the inmutatable scoreQueue of that player
	std::vector<std::vector<Box>> const* getMap() {return &boxArray;}	//get the inmutatable map
	bool tryCapture(const int& playerIndex, const int& row, const int& col);	//capture that box for that player
	bool isEnded() {return available_Box == 0;}	//is the game ended?
	void debug_print()	//print the map on console
	{
		for (auto& boxRow : boxArray)
		{
			for (Box& box : boxRow)
			{
				if (box.state == Box::wall)
					std::cout << "x";
				else if (box.state == Box::occupied)
					std::cout << box.playerIndex;
				else
					std::cout << "o";
			}
			std::cout << std::endl;
		}	
	}
};