#pragma once
#include "game.h"
#include <map>
#include <string>


class GameGUI : public sf::Drawable
{
public:
	enum State{InGame, GameMenu, Exited};
private:
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