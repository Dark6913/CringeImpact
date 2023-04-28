#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "MapObject.hpp"
#include "Cursor.hpp"

class Client
{
public:
	Client();
	void run();
private:
	sf::RenderWindow m_window;
	Cursor m_cursor;
	World m_world;
	sf::View m_camera;
	sf::Font m_font;
	sf::Text m_fps;

	void drawInterface();
	sf::Vector2f getRelativeCursorPosition();
	sf::Vector2f getAbsoluteCursorPosition();
	sf::IntRect getCameraRect();
};