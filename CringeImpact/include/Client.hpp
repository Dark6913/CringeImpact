#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "MapObject.hpp"

class Client
{
public:
	Client();
	void run();
private:
	sf::RenderWindow m_window;
	sf::Image m_cursor_image;
	sf::Cursor m_cursor;
	sf::View m_camera;
	sf::Font m_font;
	sf::Text m_fps;

	void drawInterface();
	sf::Vector2f getRelativeCursorPosition();
	sf::Vector2f getAbsoluteCursorPosition();
	sf::IntRect getCameraRect();

	std::list<std::string> m_console_log;
	std::string m_console_input;
	bool m_is_need_resolve;

	void renderConsole();
	void handleCommand(std::string line);
};