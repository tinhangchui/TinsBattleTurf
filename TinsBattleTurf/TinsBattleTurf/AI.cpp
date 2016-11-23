#include "AI.h"

int Greedy::test(std::vector<std::vector<Box>> map, int row, int col, int score)
{
	int gain = score;
	Box* adjacentBox[] = {
		&map[row - 1][col],	//up
		&map[row + 1][col],	//down
		&map[row][col - 1],	//left
		&map[row][col + 1],	//right
		&map[row - 1][col - 1],//lefttop
		&map[row - 1][col + 1],//righttop
		&map[row + 1][col - 1],//leftbottom
		&map[row + 1][col + 1],//rightbottom
	};
	//check the surroundings
	for (int i = 0; i < 8; i++)
	{
		if (adjacentBox[i]->state == Box::occupied)
		{
			//if it is the same owner, fortify
			if (adjacentBox[i]->playerIndex == playerIndex)
			{
				gain += 3;
			}
			//else, if the targetBox.score > adjacentBox[i], capture that box
			else if (score > adjacentBox[i]->score)
			{
				gain += adjacentBox[i]->score;
			}
		}
	}
	return gain;
}

sf::Vector2i Greedy::play(std::vector<std::vector<Box>> const * mapPtr, int score)
{
	sf::Vector2i highest;
	int highest_gain = score;
	for (int i = 0; i < (*mapPtr).size(); i++)
	{
		for (int j = 0; j < (*mapPtr)[i].size(); j++)
		{
			if ((*mapPtr)[i][j].state == Box::State::empty)
			{
				int gain = test(*mapPtr, i, j, score);
				if (gain > highest_gain)
				{
					highest = sf::Vector2i(i, j);
					highest_gain = gain;
				}
			}
		}
	}
	//if no gain, just place it somewhere
	if (highest_gain == score)
	{
		srand(time(NULL));
		int i = 0, j = 0;
		do {
			i = rand() % (*mapPtr)[0].size();
			j = rand() % (*mapPtr).size();
		} while ((*mapPtr)[i][j].state != Box::State::empty);
		highest = sf::Vector2i(i, j);
	}
	return highest;
}
