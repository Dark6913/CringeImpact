#pragma once
#include <unordered_map>
#include <tinytmx.hpp>
#include "Solid.hpp"
#include "MapObject.hpp"

class World
{
private:
	struct TileAnimation
	{
		uint8_t cur_frame;
		uint32_t last_clock_value;
	};

	class MapSolid : public Solid
	{
	public:
		void addCollBox(sf::IntRect rect);
	};

public:

	enum SurfaceType : int
	{
		ST_UNKNOWN = -1,
		ST_GRASS,
		ST_ROAD
	};

	static void loadFromFile(std::string path);
	static void setCameraRect(sf::IntRect camera_rect);
	static std::list<MapObject*>& getNatureList();
	static std::list<MapObject*>& getLootList();
	static SurfaceType getSurfaceType(sf::Vector2f coord);
	static Solid* getMapCollision();

	static sf::Vector2f getSpawnPoint()
	{
		const tinytmx::Object* obj = ((tinytmx::ObjectGroup*)m_layers.at("test"))->GetObject(0);
		return sf::Vector2f(obj->GetX() * m_map_scale.x, obj->GetY() * m_map_scale.y);
	};

	static void update();
	static void release();
	static void render(sf::RenderTarget& target);
private:
	World() {};

	static std::string m_textures_path;
	static tinytmx::Map m_map;
	static std::unordered_map<std::string, const tinytmx::Layer*> m_layers;
	static std::unordered_map<const tinytmx::Tile*, TileAnimation> m_tile_anim;
	static std::vector<sf::Texture> m_tilesets;
	static std::list<MapObject*> m_nature_list;
	static std::list<MapObject*> m_loot_list;
	static sf::Vector2i m_map_scale;
	static sf::Clock m_clock;
	static sf::IntRect m_camera_rect;
	static MapSolid m_map_solid;

	static void loadMapCollision();
	static void loadNature();
	static void loadLoot();
};