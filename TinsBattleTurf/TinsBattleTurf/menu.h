#pragma once

#include <SFML/Graphics.hpp>
#include "game.h"
#include "gmaeGUI.h"

/*
Button class
for menu's buttons
*/
class Button : public sf::RectangleShape
{
private:
	//the pointers to texture
	sf::Texture texture;
	sf::Texture texture_focus;
public:
	Button() { ; }
	//set the texture of button(original texture, and focus texture)
	void setButtonTexture(const std::string& original_filename, const std::string focus_filename);
	//is cursor on the button?
	bool isCursor_on_button(const sf::Vector2i);
	//change texture to normal
	void change_texture_to_normal() { this->setTexture(&texture); }
	//change texture to focus
	void change_texture_to_focus() { this->setTexture(&texture_focus); }
};
/*
Menu class
*/
class Menu : public sf::Drawable
{
public:
	enum State{Main, Host, Client, Exited, Ready};
private:
	State state;
	//main state
	sf::Texture tex_gameLogo;
	sf::RectangleShape gameLogo;
	Button hostButton;
	Button clientButton;
	Button exitButton;
	//host state
	Button backButton;
	Button startButton;
public:
	Menu();
	void initialize();	//load the textures and initialize the settings
	void mouseClick(sf::Vector2i& mousePosition);	//when mouse click, do something...
	void mouseMove(sf::Vector2i& mousePosition);	//when mouse move, do something...
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;	//draw the menu
	State getState() { return state; }
	GameGUI* buildGame();	//build game according the info in menu
};