#pragma once
#include <SFML/Graphics.hpp>

class Cursor : public sf::Drawable
{
public:
	enum CursorStyle : uint32_t
	{
		ARROW,
		POINTER,
		HAND,
		GRAP
	};

	Cursor();
	void loadFromFile(std::string file);
	void setPosition(sf::Vector2f position);
	void setCursorStyle(CursorStyle style);
private:
	sf::Vector2u m_cursor_size;
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};