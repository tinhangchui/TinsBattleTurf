#pragma once
#include "game.h"
#include "AI.h"
#include <map>
#include <string>
#include <chrono>
#include <thread>


class GameGUI : public sf::Drawable
{
public:
	enum State{InGame, GameMenu, Exited};
protected:
	State state;
	Game* gamePtr;
	std::vector<std::vector<Box>> const* mapPtr;

	sf::Font font;
	std::map<std::string, sf::Texture> textureMap;
	std::vector<std::vector<sf::RectangleShape>> mapSprite;
	std::vector<std::vector<sf::Text>> mapText;
	sf::RectangleShape* previousBox;

	int playerIndex;
	int num_player;
	sf::RectangleShape userBox;
	sf::Text userBox_Text;
	sf::RectangleShape winnerBox;
	sf::RectangleShape gameMenuRect;
	std::vector<sf::Text> gameMenu_playerScore;

	static const std::string UserBoxTexture[4];
	static const std::string PlayerFilePrefix[4];
	const sf::Texture* _getTexture(const std::string& str);
	void _updateMap();
public:
	GameGUI(Game* game);
	~GameGUI();
	State getState() { return state; }
	virtual void mouseMove(const sf::Vector2i& mousePosition);
	virtual void mouseClick(const sf::Vector2i& mousePosition);
	virtual void keyPress();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

class GameGUI_AI : public GameGUI
{
private:
	MiniMaxAI greedAI;
public:
	GameGUI_AI(Game* game) : GameGUI::GameGUI(game), greedAI(1)
	{
		;
	}
	virtual void mouseClick(const sf::Vector2i& mousePosition)
	{
		//if outside, return
		if (mousePosition.x / 50 >= (*mapPtr).size() || mousePosition.y / 50 >= (*mapPtr)[0].size())
			return;

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
		while (!gamePtr->isEnded() && playerIndex != 0)
		{
			//std::this_thread::sleep_for(std::chrono::seconds(1));
			sf::Vector2i result = greedAI.play(gamePtr->getMap(), gamePtr->getScoreQueue(playerIndex)->front());
			gamePtr->tryCapture(playerIndex, result.x, result.y);
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
				return;
			}
		}
	}
};