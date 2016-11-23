#include "menu.h"

void Button::setButtonTexture(const std::string& original_filename, const std::string focus_filename)
{
	texture.loadFromFile(original_filename);
	texture_focus.loadFromFile(focus_filename);
}

bool Button::isCursor_on_button(const sf::Vector2i mouseposition)
{
	return mouseposition.x > this->getPosition().x &&
		mouseposition.y > this->getPosition().y &&
		mouseposition.x < (this->getPosition().x + this->getSize().x) &&
		mouseposition.y < (this->getPosition().y + this->getSize().y);
}

Menu::Menu()
{
	initialize();
}

void Menu::initialize()
{
	state = State::Main;

	tex_gameLogo.loadFromFile("Texture/image_text_3_2.png");
	gameLogo.setSize(sf::Vector2f(400, 300));
	gameLogo.setTexture(&tex_gameLogo);
	gameLogo.setPosition(100, 30);

	clientButton.setButtonTexture("Texture/button_client_4_1.png", "Texture/button_client_focus_4_1.png");
	clientButton.change_texture_to_normal();
	clientButton.setSize(sf::Vector2f(200, 50));
	clientButton.setPosition(200, 350);

	hostButton.setButtonTexture("Texture/button_host_4_1.png", "Texture/button_host_focus_4_1.png");
	hostButton.change_texture_to_normal();
	hostButton.setSize(sf::Vector2f(200, 50));
	hostButton.setPosition(200, 425);

	exitButton.setButtonTexture("Texture/button_exit_2_1.png", "Texture/button_exit_focus_2_1.png");
	exitButton.change_texture_to_normal();
	exitButton.setSize(sf::Vector2f(200, 50));
	exitButton.setPosition(200, 500);

	backButton.setButtonTexture("Texture/button_back_4_1.png", "Texture/button_back_focus_4_1.png");
	backButton.change_texture_to_normal();
	backButton.setSize(sf::Vector2f(200, 50));
	backButton.setPosition(50, 500);

	startButton.setButtonTexture("Texture/button_start_2_1.png", "Texture/button_start_focus_2_1.png");
	startButton.change_texture_to_normal();
	startButton.setSize(sf::Vector2f(200, 50));
	startButton.setPosition(350, 500);
}

void Menu::mouseClick(sf::Vector2i & mousePosition)
{
	if (state == State::Main)
	{
		if (clientButton.isCursor_on_button(mousePosition))
			state = State::Client;
		else if (hostButton.isCursor_on_button(mousePosition))
			state = State::Host;
		else if (exitButton.isCursor_on_button(mousePosition))
			state = State::Exited;
	}
	else if (state == State::Host)
	{
		if (backButton.isCursor_on_button(mousePosition))
			state = State::Main;
		else if (startButton.isCursor_on_button(mousePosition))
			state = State::Ready;
	}
}

void Menu::mouseMove(sf::Vector2i & mousePosition)
{
	hostButton.change_texture_to_normal();
	clientButton.change_texture_to_normal();
	exitButton.change_texture_to_normal();

	if (clientButton.isCursor_on_button(mousePosition))
		clientButton.change_texture_to_focus();
	else if (hostButton.isCursor_on_button(mousePosition))
		hostButton.change_texture_to_focus();
	else if (exitButton.isCursor_on_button(mousePosition))
		exitButton.change_texture_to_focus();
	else if (backButton.isCursor_on_button(mousePosition))
		backButton.change_texture_to_focus();
	else if (startButton.isCursor_on_button(mousePosition))
		startButton.change_texture_to_focus();
}

void Menu::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	switch (state)
	{
	case(Main):
		target.draw(gameLogo);
		target.draw(hostButton);
		target.draw(clientButton);
		target.draw(exitButton);
		break;
	case(Host):
		target.draw(backButton);
		target.draw(startButton);
		break;
	case(Client):
		break;
	case(Exited):
	default:
		break;
	}
}

GameGUI * Menu::buildGame()
{
	return new GameGUI_AI(new Game(2, 12, 12, 20));
}

