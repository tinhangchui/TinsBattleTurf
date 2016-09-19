#include "gmaeGUI.h"

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

GameGUI::GameGUI(Game * game) : gamePtr(game), previousBox(nullptr)
{
	//load font
	font.loadFromFile("Texture/arial.ttf");
	//load map
	mapPtr = game->getMap();
	mapSprite.resize(mapPtr->size());
	for (int i = 0; i < mapSprite.size(); i++)
	{
		mapSprite[i].resize(mapPtr[0].size());
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
		}
	}
}

GameGUI::~GameGUI()
{
	delete gamePtr;
}

void GameGUI::mouseMove(const sf::Vector2i & mousePosition)
{
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
}

void GameGUI::keyPress()
{

}

void GameGUI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto& spriteRow : mapSprite)
	{
		for (const sf::RectangleShape& sprite : spriteRow)
		{
			target.draw(sprite);
		}
	}
}
