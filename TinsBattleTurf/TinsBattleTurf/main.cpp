#include <SFML/Graphics.hpp>
#include "menu.h"
#include "game.h"

GameGUI* runMenu(sf::RenderWindow& window);
void runGame(sf::RenderWindow& window, GameGUI* gui);

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "BattleTurf");
	GameGUI* gamePtr = nullptr;
	while((gamePtr = runMenu(window)) != nullptr)
	{
		runGame(window, gamePtr);
		delete gamePtr;
	}
	return 0;
}

/*
runMenu function
run menu, return game object
if game* is null, that means the player exit the menu
*/
GameGUI* runMenu(sf::RenderWindow& window)
{
	Menu menu;

	while (window.isOpen())
	{
		sf::Event event;
		//update event
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();			
			if (event.type == sf::Event::MouseButtonPressed)
			{
				menu.mouseClick(sf::Mouse::getPosition(window));
			}
			if (event.type == sf::Event::MouseMoved)
			{
				menu.mouseMove(sf::Mouse::getPosition(window));
			}
		}
		//update menu state
		if (menu.getState() == Menu::State::Exited)
			window.close();
		if (menu.getState() == Menu::State::Ready)
			return menu.buildGame();

		window.clear(sf::Color::White);
		window.draw(menu);
		window.display();
	}
	return nullptr;
}

void runGame(sf::RenderWindow& window, GameGUI * gui)
{
	while(window.isOpen() && gui->getState() != GameGUI::State::Exited)
	{
		sf::Event event;
		//update event
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed)
			{
				gui->mouseClick(sf::Mouse::getPosition(window));
			}
			if (event.type == sf::Event::MouseMoved)
			{
				gui->mouseMove(sf::Mouse::getPosition(window));
			}
		}

		window.clear(sf::Color::White);
		window.draw(*gui);
		window.display();
	}
}
