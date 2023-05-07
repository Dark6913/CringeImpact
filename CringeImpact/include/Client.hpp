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
	sf::Cursor m_cursor;
	sf::View m_camera;
	sf::Font m_font;
	sf::Text m_fps;

	void drawInterface();
	sf::Vector2f getRelativeCursorPosition();
	sf::Vector2f getAbsoluteCursorPosition();
	sf::IntRect getCameraRect();
};