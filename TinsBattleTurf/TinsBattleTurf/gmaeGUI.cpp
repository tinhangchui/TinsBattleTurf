#include "gmaeGUI.h"

const std::string GameGUI::UserBoxTexture[4] = {"Texture/box_blue_1_22.png",
											  "Texture/box_red_1_22.png",
											  "Texture/box_green_1_22.png",
											  "Texture/box_yellow_1_22.png" };
const std::string GameGUI::PlayerFilePrefix[4] = { "box_blue_1_", "box_red_1_", "box_green_1_", "box_yellow_1_" };

const sf::Texture * GameGUI::_getTexture(const std::string & str)
{
	auto it = textureMap.find(str);
	if (it != textureMap.end())
		return &(it->second);
	sf::Texture texture;
	texture.loadFromFile(str);
	textureMap.emplace(str, texture);
	return &textureMap[str];
}

void GameGUI::_updateMap()
{
	
	for (int i = 0; i < mapPtr->size(); i++)
	{
		for (int j = 0; j < (*mapPtr)[0].size(); j++)
		{
			if ((*mapPtr)[i][j].state == Box::empty)
				mapSprite[i][j].setTexture(_getTexture("Texture/box_empty_1_1.png"));
			else if((*mapPtr)[i][j].state == Box::wall)
				mapSprite[i][j].setTexture(_getTexture("Texture/box_wall_1_1.png"));
			else if ((*mapPtr)[i][j].state == Box::occupied)
			{
				/*declare the hash value, initialize as 0.
				If the player also owns the top, the hash value +1
				If the player also owns the right, the hash value +5
				If the player also owns the bottom, the hash value +7
				If the player also owns the left, the hash value +9
				*/
				int hashNum = 0;
				if ((*mapPtr)[i][j].playerIndex != (*mapPtr)[i][j - 1].playerIndex)
					hashNum += 1;
				if ((*mapPtr)[i][j].playerIndex != (*mapPtr)[i + 1][j].playerIndex)
					hashNum += 5;
				if ((*mapPtr)[i][j].playerIndex != (*mapPtr)[i][j + 1].playerIndex)
					hashNum += 7;
				if ((*mapPtr)[i][j].playerIndex != (*mapPtr)[i - 1][j].playerIndex)
					hashNum += 9;

				std::string path = "Texture/" + PlayerFilePrefix[(*mapPtr)[i][j].playerIndex] + std::to_string(hashNum) + ".png";
				mapSprite[i][j].setTexture(_getTexture(path));
				mapText[i][j].setString(std::to_string((*mapPtr)[i][j].score));
			}
				
		}
	}
}

GameGUI::GameGUI(Game * game) : gamePtr(game), previousBox(nullptr)
{
	//load font
	font.loadFromFile("Texture/arial.ttf");
	//load map
	mapPtr = game->getMap();
	mapSprite.resize(mapPtr->size());
	mapText.resize(mapPtr->size());
	for (int i = 0; i < mapSprite.size(); i++)
	{
		mapSprite[i].resize(mapPtr[0].size());
		mapText[i].resize(mapPtr[0].size());
	}

	for (int i = 0; i < mapSprite.size(); i++)
	{
		for (int j = 0; j < mapSprite[0].size(); j++)
		{
			mapSprite[i][j].setSize(sf::Vector2f(50, 50));
			mapSprite[i][j].setPosition(sf::Vector2f(50 * i, 50 * j));
			if((*mapPtr)[i][j].state != Box::wall)
				mapSprite[i][j].setTexture(_getTexture("Texture/box_empty_1_1.png"));
			else
				mapSprite[i][j].setTexture(_getTexture("Texture/box_wall_1_1.png"));
			mapText[i][j].setFont(font);
			mapText[i][j].setFillColor(sf::Color::Black);
			mapText[i][j].setPosition(sf::Vector2f(50 * i + 10, 50 * j + 7));
		}
	}
	//load playerData
	playerIndex = 0;
	num_player = game->getNumPlayer();
	//load userBox
	userBox.setSize(sf::Vector2f(50, 50));
	userBox.setTexture(_getTexture(UserBoxTexture[0]));
	int firstNum = game->getScoreQueue(0)->front();
	
	userBox_Text.setString(std::to_string(firstNum));
	userBox_Text.setFont(font);
	userBox_Text.setFillColor(sf::Color::Black);
}

GameGUI::~GameGUI()
{
	delete gamePtr;
}

void GameGUI::mouseMove(const sf::Vector2i & mousePosition)
{
	//update the position of userBox
	userBox.setPosition(mousePosition.x + 10, mousePosition.y + 10);
	userBox_Text.setPosition(mousePosition.x + 15, mousePosition.y + 15);

	//if the previousBox "exist", set that box to normal texture
	if (previousBox)
	{
		previousBox->setTexture(_getTexture("Texture/box_empty_1_1.png"));
	}

	if ((*mapPtr)[mousePosition.x / 50][mousePosition.y / 50].state == Box::empty)
	{
		//change the box where the cursor located to focus
		mapSprite[mousePosition.x / 50][mousePosition.y / 50].setTexture(_getTexture("Texture/box_focus_1_1.png"));
		//set pointer to that box, so that it can return to normal texture
		previousBox = &mapSprite[mousePosition.x / 50][mousePosition.y / 50];
	}
}

void GameGUI::mouseClick(const sf::Vector2i & mousePosition)
{
	if (gamePtr->isEnded())
		state = State::Exited;
	if (gamePtr->tryCapture(playerIndex, mousePosition.x / 50, mousePosition.y / 50))
	{
		_updateMap();
		if (!gamePtr->isEnded())
		{
			playerIndex++;
			if (playerIndex >= num_player)
				playerIndex = 0;
			userBox.setTexture(_getTexture(UserBoxTexture[playerIndex]));
			userBox_Text.setString(std::to_string(gamePtr->getScoreQueue(playerIndex)->front()));
		}
		else
		{
			int winner = 0;
			for (int i = 0; i < num_player; i++)
			{
				if (gamePtr->getScore(i) > gamePtr->getScore(winner))
					winner = i;
				winnerBox.setSize(sf::Vector2f(400, 250));
				winnerBox.setPosition(sf::Vector2f(100, 175));
				winnerBox.setTexture(_getTexture("Texture/" + PlayerFilePrefix[winner] + "win.png"));
			}
		}
	}
	previousBox = nullptr;
}

void GameGUI::keyPress()
{

}

void GameGUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (int i = 0; i < mapSprite.size(); i++)
	{
		for (int j = 0; j < mapSprite[0].size(); j++)
		{
			target.draw(mapSprite[i][j]);
			target.draw(mapText[i][j]);
		}
	}
	if (!gamePtr->isEnded())
	{
		target.draw(userBox);
		target.draw(userBox_Text);
	}
	else
	{
		target.draw(winnerBox);
	}
}
