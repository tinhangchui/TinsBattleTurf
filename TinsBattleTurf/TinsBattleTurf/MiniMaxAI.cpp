#include "AI.h"
bool MiniMaxAI::isEnd(std::vector<std::vector<Box>> const * mapPtr)
{
	for (int i = 0; i < (*mapPtr).size(); i++)
	{
		for (int j = 0; j < (*mapPtr)[i].size(); j++)
		{
			if ((*mapPtr)[i][j].state == Box::State::empty)
			{
				return false;
			}
		}
	}
	return true;
}
/*
Compute "gains" of all possible moves and then sort it. Return the first n moves (n most beneficial moves).
The size of vector will less than n if there are less than n possible moves.
*/
std::vector<sf::Vector3i> MiniMaxAI::searchMoves(std::vector<std::vector<Box>> const * mapPtr, int score, int n, bool decreasingOrder)
{
	//vector of (row, col, netGain).
	std::vector<sf::Vector3i> netChange;
	for (int i = 0; i < (*mapPtr).size(); i++)
	{
		for (int j = 0; j < (*mapPtr)[i].size(); j++)
		{
			if ((*mapPtr)[i][j].state == Box::State::empty) 
			{
				sf::Vector3i net = sf::Vector3i(i, j, test((*mapPtr), i, j, score));
				netChange.push_back(net);
			}
		}
	}
	if (decreasingOrder)
		std::sort(netChange.begin(), netChange.end(), [&](sf::Vector3i first, sf::Vector3i second) { return first.z > second.z; });
	else
		std::sort(netChange.begin(), netChange.end(), [&](sf::Vector3i first, sf::Vector3i second) { return first.z < second.z; });

	std::vector<sf::Vector3i> result;
	for (auto it = netChange.begin(); it != netChange.end() && n >= 0; it++) 
	{
		result.push_back(*it);
		n--;
	}
	return result;
}

sf::Vector3i MiniMaxAI::findMax(std::vector<std::vector<Box>> const * mapPtr, int score, int level)
{
	if (isEnd(mapPtr))
	{
		return sf::Vector3i(0, 0, -INFINITY);
	}

	if (level == 0)
	{
		std::vector<sf::Vector3i> moves = searchMoves(mapPtr, score, NUMSEARCHNODE, true);
		return moves[0];
	}

	std::vector<sf::Vector3i> netMove = searchMoves(mapPtr, score, NUMSEARCHNODE, true);
	int maxSoFar = 0;	//assume the max is the first of netMove
	for (int i = 0; i < netMove.size(); i++)
	{
		sf::Vector3i move = netMove[i];
		auto copymap = *mapPtr;
		copymap[move.x][move.y].state = Box::State::occupied;
		sf::Vector3i min = findMin(&copymap, ASSUMESCORE, level - 1);
		if (min.z > netMove[maxSoFar].z)
			maxSoFar = i;
	}
	return netMove[maxSoFar];
}

sf::Vector3i MiniMaxAI::findMin(std::vector<std::vector<Box>> const * mapPtr, int score, int level)
{
	if (isEnd(mapPtr))
	{
		return sf::Vector3i(0, 0, INFINITY);
	}

	if (level == 0)
	{
		std::vector<sf::Vector3i> moves = searchMoves(mapPtr, score, NUMSEARCHNODE, false);
		return moves[0];
	}

	std::vector<sf::Vector3i> netMove = searchMoves(mapPtr, score, NUMSEARCHNODE, true);
	int minSoFar = 0;	//assume the min is the first of netMove
	for (int i = 0; i < netMove.size(); i++)
	{
		sf::Vector3i move = netMove[i];
		auto copymap = *mapPtr;
		copymap[move.x][move.y].state = Box::State::occupied;
		sf::Vector3i min = findMin(&copymap, ASSUMESCORE, level - 1);
		if (min.z < netMove[minSoFar].z)
			minSoFar = i;
	}
	return netMove[minSoFar];
}

sf::Vector2i MiniMaxAI::play(std::vector<std::vector<Box>> const * mapPtr, int score)
{
	sf::Vector3i result = findMax(mapPtr, score, SEARCHLEVEL);
	return sf::Vector2i(result.x, result.y);
}