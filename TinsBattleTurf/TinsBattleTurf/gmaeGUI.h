#pragma once
#include "game.h"
#include <map>

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
	sf::RectangleShape* previousBox;

	const sf::Texture* _getTexture(const std::string& str);
public:
	GameGUI(Game* game);
	~GameGUI();
	State getState() { return state; }
	virtual void mouseMove(const sf::Vector2i& mousePosition);
	virtual void mouseClick(const sf::Vector2i& mousePosition);
	virtual void keyPress();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};