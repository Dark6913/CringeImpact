#include <SFML/Graphics.hpp>
#include <string.h>
#include <vector>
#include <list>
#include <iostream>
#include "Tree.hpp"
#include "Bush.hpp"
#include "Chest.hpp"
#include "World.hpp"

World::World()
{
	m_textures_path = "data/textures/";
	m_map_scale = sf::Vector2i(4, 4);
}

World::World(std::string path)
{
	m_textures_path = "data/textures/";
	m_map_scale = sf::Vector2i(4, 4);
	this->loadFromFile(path);
}

void World::loadFromFile(std::string path)
{
	m_map.ParseFile(path);
	if (m_map.IsInfinite())
	{
		std::cout << "Map is infinite! Uncheck \"Infinite\" checkbox.\n";
		exit(1);
	}

	for (int i = 0; i < m_map.GetNumLayers(); i++)
	{
		const tinytmx::Layer* layer = m_map.GetLayer(i);
		m_layers.insert(std::pair(layer->GetName(), layer));
	}

	m_tilesets.resize(m_map.GetNumTilesets());
	for (int i = 0; i < m_map.GetNumTilesets(); i++)
	{
		const tinytmx::Tileset* tileset = m_map.GetTileset(i);
		m_tilesets[i].loadFromFile(m_textures_path + tileset->GetImage()->GetSource());
		for (int j = 0; j < tileset->GetTileCount(); j++)
		{
			const tinytmx::Tile* tile = tileset->GetTile(j);
			if (tile && tile->IsAnimated())
			{
				TileAnimation anim;
				anim.cur_frame = 0;
				anim.last_clock_value = 1;
				m_tile_anim.insert(std::pair(tile, anim));
			}
		}
	}

	this->loadMapCollision();
	this->loadNature();
	this->loadLoot();
}

void World::setCameraRect(sf::IntRect camera_rect)
{
	m_camera_rect = camera_rect;
}

void World::loadMapCollision()
{
	tinytmx::TileLayer* layer = (tinytmx::TileLayer*)m_layers.at("Main tile layer");
	for (int y = 0; y < layer->GetHeight(); y++)
	{
		for (int x = 0; x < layer->GetWidth(); x++)
		{
			const tinytmx::MapTile map_tile = layer->GetDataTileFiniteMap()->GetTile(x, y);
			const tinytmx::Tileset* tileset = m_map.GetTileset(map_tile.tilesetId);
			const tinytmx::Tile* tile = tileset->GetTile(map_tile.id);
			if (tile && tile->HasObjectGroup())
			{
				sf::Vector2i offset = sf::Vector2i(
					x * (int)tileset->GetTileWidth(),
					y * (int)tileset->GetTileHeight()
				);

				const tinytmx::ObjectGroup* collision_group = tile->GetObjectGroup();
				for (int cb_id = 0; cb_id < collision_group->GetNumObjects(); cb_id++)
				{
					const tinytmx::Object* object = collision_group->GetObject(cb_id);
					m_collisions.push_back(
						sf::IntRect(
							(offset.x + object->GetX()) * m_map_scale.x,
							(offset.y + object->GetY()) * m_map_scale.y,
							object->GetWidth() * m_map_scale.x,
							object->GetHeight() * m_map_scale.y
						)
					);
				}
			}
		}
	}
}

void World::loadNature()
{
	const tinytmx::ObjectGroup* object_group = (tinytmx::ObjectGroup*)m_layers.at("Nature object layer");
	for (int i = 0; i < object_group->GetNumObjects(); i++)
	{
		const tinytmx::Object* object = object_group->GetObject(i);
		const tinytmx::Tileset* tileset = m_map.GetTileset(object->GetTile()->tilesetId);
		const tinytmx::Tile* tile = tileset->GetTile(object->GetTile()->id);
		MapObject* map_object = NULL;

		if (object->GetClass() == "tree") map_object = new Tree();
		else if (object->GetClass() == "bush") map_object = new Bush();

		if (map_object != NULL)
		{
			map_object->setPosition(sf::Vector2f(object->GetX() * m_map_scale.x, object->GetY() * m_map_scale.y));
			if (tile && tile->IsAnimated())
			{
				map_object->loadAnimation(
					m_tilesets[object->GetTile()->tilesetId],
					tile->GetFrameCount(), 1,
					tile->GetFrames()[0].GetDuration() / 1000.f,
					(sf::Vector2f)m_map_scale,
					sf::Vector2f(0.f, 1.f)
				);
				m_tile_anim.erase(tile);
			}
			if (tile && tile->HasObjects())
			{
				const tinytmx::ObjectGroup* collision_group = tile->GetObjectGroup();
				for (int j = 0; j < collision_group->GetNumObjects(); j++)
				{
					const tinytmx::Object* coll_box = collision_group->GetObject(j);
					map_object->addCollisionRect(
						sf::IntRect(
							coll_box->GetX() * m_map_scale.x,
							coll_box->GetY() * m_map_scale.y,
							coll_box->GetWidth() * m_map_scale.x,
							coll_box->GetHeight() * m_map_scale.y
						)
					);
				}
			}
			m_nature_list.push_back(map_object);
		}
	}
}

void World::loadLoot()
{
	const tinytmx::ObjectGroup* object_group = (tinytmx::ObjectGroup*)m_layers.at("Loot object layer");
	for (int i = 0; i < object_group->GetNumObjects(); i++)
	{
		const tinytmx::Object* object = object_group->GetObject(i);
		const tinytmx::Tileset* tileset = m_map.GetTileset(object->GetTile()->tilesetId);
		const tinytmx::Tile* tile = tileset->GetTile(object->GetTile()->id);
		MapObject* map_object = NULL;

		map_object = new Chest();

		if (map_object != NULL)
		{
			map_object->setPosition(sf::Vector2f(object->GetX() * m_map_scale.x, object->GetY() * m_map_scale.y));
			if (tile && tile->IsAnimated())
			{
				map_object->loadAnimation(
					m_tilesets[object->GetTile()->tilesetId],
					tile->GetFrameCount(), 1,
					tile->GetFrames()[0].GetDuration() / 2000.f,
					(sf::Vector2f)m_map_scale,
					sf::Vector2f(0.f, 1.f)
				);
				m_tile_anim.erase(tile);
			}
			if (tile && tile->HasObjects())
			{
				const tinytmx::ObjectGroup* collision_group = tile->GetObjectGroup();
				for (int j = 0; j < collision_group->GetNumObjects(); j++)
				{
					const tinytmx::Object* coll_box = collision_group->GetObject(j);
					map_object->addCollisionRect(
						sf::IntRect(
							coll_box->GetX() * m_map_scale.x,
							coll_box->GetY() * m_map_scale.y,
							coll_box->GetWidth() * m_map_scale.x,
							coll_box->GetHeight() * m_map_scale.y
						)
					);
				}
			}
			m_loot_list.push_back(map_object);
		}
	}
}

std::list<MapObject*>& World::getNatureList()
{
	return m_nature_list;
}

std::list<MapObject*>& World::getLootList()
{
	return m_loot_list;
}

std::string World::getTileType(sf::Vector2f coord)
{
	tinytmx::TileLayer* tile_layer = (tinytmx::TileLayer*)m_layers.at("Main tile layer");
	sf::Vector2i tile_coord = { 
		(int)coord.x / (int)m_map.GetTileWidth() / m_map_scale.x,
		(int)coord.y / (int)m_map.GetTileHeight() / m_map_scale.y
	};
	int tileset_id = tile_layer->GetDataTileFiniteMap()->GetTile(tile_coord.x, tile_coord.y).tilesetId;
	const std::string& source = m_map.GetTileset(tileset_id)->GetImage()->GetSource();
	size_t offset = source.rfind('/');
	offset = (offset == source.npos) ? 0 : offset + 1;
	return source.substr(offset, source.rfind('.') - offset);
}

void World::update()
{
	uint32_t clock_value = m_clock.getElapsedTime().asMilliseconds();
	for (auto it = m_tile_anim.begin(); it != m_tile_anim.end(); it++)
	{
		TileAnimation anim = it->second;
		if (clock_value - anim.last_clock_value >= it->first->GetFrames()[anim.cur_frame].GetDuration())
		{
			anim.cur_frame++;
			if (anim.cur_frame >= it->first->GetFrameCount()) anim.cur_frame = 0;
			anim.last_clock_value = clock_value;
			m_tile_anim.insert_or_assign(it->first, anim);
		}
	}
}

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Sprite sprite;
	tinytmx::TileLayer* layer = (tinytmx::TileLayer*)m_layers.at("Main tile layer");
	int left_tile_id = m_camera_rect.left / (int)m_map.GetTileWidth() / m_map_scale.x;
	int top_tile_id = m_camera_rect.top / (int)m_map.GetTileHeight() / m_map_scale.y;
	int screen_tile_width = m_camera_rect.width / m_map.GetTileWidth() / m_map_scale.x + 2;
	int screen_tile_height = m_camera_rect.height / m_map.GetTileHeight() / m_map_scale.y + 2;

	for (int y = top_tile_id; y < top_tile_id + screen_tile_height; y++)
	{
		for (int x = left_tile_id; x < left_tile_id + screen_tile_width; x++)
		{
			if (y < 0 || x < 0 || x >= layer->GetWidth() || y >= layer->GetHeight()) continue;
			tinytmx::MapTile map_tile = layer->GetDataTileFiniteMap()->GetTile(x, y);
			const tinytmx::Tileset* tileset = m_map.GetTileset(map_tile.tilesetId);
			const tinytmx::Tile* tile = tileset->GetTile(map_tile.id);
			if (tile && tile->IsAnimated()) map_tile.id += m_tile_anim.at(tile).cur_frame;
			uint32_t cols = tileset->GetColumns();
			sf::IntRect tile_rect = sf::IntRect(
				map_tile.id % cols * tileset->GetTileWidth(),
				map_tile.id / cols * tileset->GetTileHeight(),
				tileset->GetTileWidth(),
				tileset->GetTileHeight()
			);
			sprite.setTexture(m_tilesets[map_tile.tilesetId]);
			sprite.setTextureRect(tile_rect);
			sprite.setScale((sf::Vector2f)m_map_scale);
			sprite.setPosition(
				x * m_map_scale.x * (int)tileset->GetTileWidth(),
				y * m_map_scale.y * (int)tileset->GetTileHeight()
			);

			target.draw(sprite, states);
		}
	}
}

void World::release()
{
	for (auto it = m_nature_list.begin(); it != m_nature_list.end(); it++)
		delete *it;
	for (auto it = m_loot_list.begin(); it != m_loot_list.end(); it++)
		delete* it;
}