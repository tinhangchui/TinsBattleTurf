#pragma once
#include "game.h"
#include <SFML/Graphics.hpp>
#include <time.h>
#include <stdio.h>

/*
Abstract AI class that can play a move.
*/
class AI
{
protected:
	int playerIndex;
public:
	AI() { ;}
	AI(int playerIndex) { this->playerIndex = playerIndex; }
	virtual sf::Vector2i play(std::vector<std::vector<Box>> const* mapPtr, int score) = 0;
};

/*
AI that search for the one that has highest gain.
*/
class Greedy : public AI
{
protected:
	//return the socre if AI plays here...
	int test(std::vector<std::vector<Box>> map, int row, int col, int score);
public:
	Greedy(int playerIndex) : AI::AI(playerIndex) { ; }
	//Return AI's move as vector2i, mapPtr is the map, score is the score of the block that AI needed to put into the map.
	sf::Vector2i play(std::vector<std::vector<Box>> const* mapPtr, int score);
};

/*
!!!Only available for two player game!!!
AI that use minimax tree to calculate the best move.
*/
class MiniMaxAI : public Greedy
{
private:
	static const int SEARCHLEVEL = 2;
	static const int NUMSEARCHNODE = 4;
	static const int ASSUMESCORE = 20;	//since there is no way to know opponent's block, so assume opponent has 20 everytime.
	bool isEnd(std::vector<std::vector<Box>> const* mapPtr);
	std::vector<sf::Vector3i> searchMoves(std::vector<std::vector<Box>> const* mapPtr, int score, int n, bool decreasingOrder);
	sf::Vector3i findMax(std::vector<std::vector<Box>> const* mapPtr, int score, int level);
	sf::Vector3i findMin(std::vector<std::vector<Box>> const* mapPtr, int score, int level);

public:
	MiniMaxAI(int playerIndex) : Greedy::Greedy(playerIndex) { ; }
	sf::Vector2i play(std::vector<std::vector<Box>> const* mapPtr, int score);
};