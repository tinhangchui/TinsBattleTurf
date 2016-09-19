#include "game.h"

void Game::helper_randomize(std::vector<int>& vector)
{
	for (int i = 0; i < vector.size(); i++)
	{
		int temp = vector[i];
		int target = rand() % vector.size();
		vector[i] = vector[target];
		vector[target] = temp;
	}
}

Game::Game(int num_player, int MAP_WIDTH, int MAP_HEIGHT, int MAP_NUM_WALL)
{
	//initialize playerList and playerscore
	assert(num_player > 0 && num_player <= 4);	//only maximum 4 players
	playerList.resize(num_player);
	playerscore.resize(num_player);
	//initialize boxArray(map)
	assert(MAP_WIDTH > 3 && MAP_HEIGHT > 3);
	available_Box = MAP_HEIGHT * MAP_WIDTH;
	boxArray.resize(MAP_HEIGHT);
	for (std::vector<Box>& boxRow : boxArray)
		boxRow.resize(MAP_WIDTH);
	for (auto& boxRow : boxArray)
		for (Box& box : boxRow)
			box.state = Box::empty;
	//initialize the currentPlayer index
	currentPlayer = 0;

	//the edge are walls
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		boxArray[i][0].state = Box::wall;
		boxArray[i][MAP_HEIGHT - 1].state = Box::wall;
		available_Box = available_Box - 2;
	}
	for (int i = 0; i < MAP_WIDTH; i++)
	{
		boxArray[0][i].state = Box::wall;
		boxArray[MAP_WIDTH - 1][i].state = Box::wall;
		available_Box = available_Box - 2;
	}
	//generate walls
	assert(available_Box > MAP_NUM_WALL);	//no room for walls
	srand(time(NULL));
	while (MAP_NUM_WALL > 0)
	{
		int row = rand() % MAP_HEIGHT;
		int col = rand() % MAP_WIDTH;
		if (boxArray[row][col].state != Box::wall)
		{
			boxArray[row][col].state = Box::wall;
			MAP_NUM_WALL--;
		}
	}
	//generate player's score
	int boxNeeded = available_Box;
	std::vector<int> temp;
	while(boxNeeded > 0)
	{
		temp.push_back(rand() % 5 + 5);
		temp.push_back(rand() % 5 + 10);
		temp.push_back(rand() % 5 + 15);
		temp.push_back(rand() % 5 + 20);
		temp.push_back(rand() % 6 + 25);
		boxNeeded -= 5;
	}
	//assign score to player's scoreList
	for (int i = 0; i < playerList.size(); i++)
	{
		helper_randomize(temp);
		for (int num : temp)
		{
			playerscore[i].push(num);
		}
	}
}

int Game::getScore(const int & playerIndex)
{
	return playerList[playerIndex].getScore();
}

std::queue<int> const* Game::getScoreQueue(int playerIndex) 
{
	return &playerscore[playerIndex];
}

bool Game::tryCapture(const int & playerIndex, const int & row, const int & col)
{
	if (playerIndex != currentPlayer || boxArray[row][col].state != Box::empty)
		return false;

	assert(!playerscore[playerIndex].empty());
	int num = playerscore[playerIndex].front();
	boxArray[row][col].score = num;
	boxArray[row][col].playerIndex = playerIndex;
	boxArray[row][col].state = Box::occupied;
	playerList[playerIndex].addScore(num);
	playerscore[playerIndex].pop();

	//define the adjacent boxes
	Box* adjacentBox[] = {
		&boxArray[row - 1][col],	//up
		&boxArray[row + 1][col],	//down
		&boxArray[row][col - 1],	//left
		&boxArray[row][col + 1],	//right
		&boxArray[row - 1][col - 1],//lefttop
		&boxArray[row - 1][col + 1],//righttop
		&boxArray[row + 1][col - 1],//leftbottom
		&boxArray[row + 1][col + 1],//rightbottom
	};
	//check the surroundings
	for (int i = 0; i < 8; i++)
	{
		if (adjacentBox[i]->state == Box::occupied)
		{
			//if it is the same owner, fortify
			if (adjacentBox[i]->playerIndex == playerIndex)
			{
				//fortify
				adjacentBox[i]->score += 3;
				//add score to the owner
				playerList[playerIndex].addScore(3);
			}
			//else, if the targetBox.score > adjacentBox[i], capture that box
			else if (boxArray[row][col].score > adjacentBox[i]->score)
			{
				//take the score away from origin player
				int score = adjacentBox[i]->score;
				int originPlayerIndex = adjacentBox[i]->playerIndex;
				playerList[originPlayerIndex].addScore(-score);

				//reset the owner
				adjacentBox[i]->playerIndex = playerIndex;

				//add score to the new owner
				playerList[playerIndex].addScore(score);
			}		
		}
	}
	//change the currentPlayerIndex to next Player
	if (currentPlayer < playerList.size())
		currentPlayer++;
	else
		currentPlayer = 0;

	return true;
}