#pragma once
#include <unordered_map>
#include <tinytmx.hpp>
#include "Solid.hpp"
#include "MapObject.hpp"

class World : public sf::Drawable, public Solid
{
public:
	World();
	World(std::string path);
	void loadFromFile(std::string path);
	void setCameraRect(sf::IntRect camera_rect);
	std::list<MapObject*>& getNatureList();
	std::list<MapObject*>& getLootList();

	sf::Vector2f getSpawnPoint()
	{
		const tinytmx::Object* obj = ((tinytmx::ObjectGroup*)m_layers.at("test"))->GetObject(0);
		return sf::Vector2f(obj->GetX() * m_map_scale.x, obj->GetY() * m_map_scale.y);
	};

	void update();
	void release();
private:

	struct TileAnimation
	{
		uint8_t cur_frame;
		uint32_t last_clock_value;
	};

	std::string m_textures_path;
	tinytmx::Map m_map;
	std::unordered_map<std::string, const tinytmx::Layer*> m_layers;
	std::unordered_map<const tinytmx::Tile*, TileAnimation> m_tile_anim;
	std::vector<sf::Texture> m_tilesets;
	std::list<MapObject*> m_nature_list;
	std::list<MapObject*> m_loot_list;
	sf::Vector2i m_map_scale;
	sf::Clock m_clock;
	sf::IntRect m_camera_rect;

	void loadMapCollision();
	void loadNature();
	void loadLoot();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};