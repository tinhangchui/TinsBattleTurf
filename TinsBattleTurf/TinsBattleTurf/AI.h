#pragma once
#include "game.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>

class AI
{
protected:
	int playerIndex;
public:
	AI() { ;}
	AI(int playerIndex) { this->playerIndex = playerIndex; }
	virtual sf::Vector2i play(std::vector<std::vector<Box>> const* mapPtr, int score) = 0;
};

class Greedy : public AI
{
private:
	//return the socre if AI plays here...
	int test(std::vector<std::vector<Box>> map, int row, int col, int score);
public:
	Greedy(int playerIndex) : AI::AI(playerIndex) { ; }
	sf::Vector2i play(std::vector<std::vector<Box>> const* mapPtr, int score);
};