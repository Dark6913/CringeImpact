#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class Animation : public sf::Drawable
{
public:
	Animation();

	// Load animation from file
	void loadFromFile(std::string file, uint8_t frames_cnt, uint8_t rows_cnt, float frame_duration);

	// Load animation from existing texture
	void loadFromMemory(sf::Texture& texture, uint8_t frames_cnt, uint8_t rows_cnt, float frame_duration);

	// Set animation position
	void setPosition(sf::Vector2f position);

	// Set animation texture scale 
	void setScale(sf::Vector2f scale);

	// Set origin offset in percents (from 0 to 1) 
	void setOrigin(sf::Vector2f offset);

	// Set current frame for animation
	void setCurrentFrame(uint8_t frame);

	// Set current row for animation
	void setCurrentRow(uint8_t row);

	// Plays animation in straight order in loop
	void playLoopInStraightOrder(float tick);

	// Plays animation in reverse order in loop
	void playLoopInReverseOrder(float tick);

	// Plays animation in straight order
	// While playing returns false
	// After finish playing returns true
	bool playInStraightOrder(float tick);

	// Plays animation in reverse order
	// While playing returns false
	// After finish playing returns true
	bool playInReverseOrder(float tick);

	// Reset frame counter
	void reset();

	// Release all allocated textures
	static void release();

	// Returns rectangle with position and size of frame in pixels 
	sf::FloatRect getGlobalBounds() { return m_sprite.getGlobalBounds(); };

	// Returns texture scale
	sf::Vector2f getScale() { return m_sprite.getScale(); };

	// Returns origin offset in pixels
	sf::Vector2f getOriginOffset() { return m_sprite.getOrigin(); };
private:
	static std::unordered_map<std::string, sf::Texture*> m_textures;
	sf::Sprite m_sprite;
	int8_t m_cur_frame;
	int8_t m_cur_row;
	int8_t m_frames_count;
	int8_t m_rows_count;
	sf::Vector2f m_origin_offset;
	float m_frame_duration;
	float m_time;

	// Function that render animation to window
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Resolve texture rectangle
	void update();
};