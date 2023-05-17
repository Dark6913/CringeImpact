#include "Client.hpp"
#include "World.hpp"
#include "Chest.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "SoundRegister.hpp"
#include "Settings.hpp"

#include <imgui.h>
#include <imgui-sfml.h>

#define CRINGE_IMPACT_VERSION	"0.2.2a"

Client::Client()
{
	SoundRegister::loadAllFromDirectory("data/audio/sound/");
	World::loadFromFile("data/map/map.tmx");
	m_window.create(sf::VideoMode(1600, 800), std::string("Cringe Impact v") + CRINGE_IMPACT_VERSION);
	ImGui::SFML::Init(m_window);

	m_cursor_image.loadFromFile("data/textures/pointer.png");
	m_cursor.loadFromPixels(m_cursor_image.getPixelsPtr(), m_cursor_image.getSize(), sf::Vector2u(0, 0));
	m_window.setMouseCursor(m_cursor);

	m_camera.setSize((sf::Vector2f)m_window.getSize());
	m_camera.setCenter(World::getSpawnPoint());

	m_font.loadFromFile("data/fonts/Jost-Regular.ttf");
	m_fps.setFont(m_font);
	m_fps.setFillColor(sf::Color(255, 255, 255));
	m_fps.setPosition(sf::Vector2f(50, 10));
	m_fps.setCharacterSize(20u);

	m_is_need_resolve = false;
}

void Client::run()
{
	std::list<MapObject*>& nature = World::getNatureList();
	std::list<MapObject*>& loot = World::getLootList();
	std::list<Solid*> solid_objects;
	std::list<IAnimated*> animated_objects;
	std::list<Entity*> enemy_list;
	std::list<Entity*> players_list;
	solid_objects.push_back(World::getMapCollision());
	for (auto it = nature.begin(); it != nature.end(); it++)
	{
		solid_objects.push_back((Solid*)*it);
		animated_objects.push_back((IAnimated*)*it);
	}
	for (auto it = loot.begin(); it != loot.end(); it++)
	{
		solid_objects.push_back((Solid*)*it);
		animated_objects.push_back((IAnimated*)*it);
	}
	
	// THE BUG
	// sf::Vector2f(3357.49878, 9413.87305)
	Player player;
	player.setPosition(World::getSpawnPoint());
	animated_objects.push_back((IAnimated*)&player);
	solid_objects.push_back((Solid*)&player);
	players_list.push_back((Entity*)&player);

	sf::CircleShape cs;
	cs.setFillColor(sf::Color::Transparent);
	cs.setOutlineColor(sf::Color::Red);
	cs.setRadius(1000.f);
	cs.setOrigin(sf::Vector2f(1000.f, 1000.f));
	cs.setPosition(sf::Vector2f(2600, 3500));
	cs.setOutlineThickness(1);

	for (int i = 0; i < 5; i++)
	{
		Enemy* enemy = new Enemy();
		enemy->setPosition(RandomVector(cs.getPosition(), cs.getRadius()));
		enemy->setLivingArea(cs.getPosition(), cs.getRadius());
		enemy_list.push_back(enemy);
		animated_objects.push_back((IAnimated*)enemy);
		solid_objects.push_back((Solid*)enemy);
	}

	float respawn_timer = 3.8f;

	sf::Clock clock;
	while (Settings::is_game_runing)
	{
		sf::Time elapsed_time = clock.restart();
		float tick = elapsed_time.asSeconds();
		m_fps.setString("FPS: " + std::to_string((int)round(1.f / tick)));

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed) Settings::is_game_runing = false;
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
				{
					Settings::is_console_shown = !Settings::is_console_shown;
					if (!Settings::is_console_shown) m_window.setMouseCursor(m_cursor);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				{
					for (auto it = loot.begin(); it != loot.end(); it++)
					{
						Chest* chest = (Chest*)*it;
						if (VectorModule(chest->getCenter() - player.getPosition()) <= 50.f)
						{
							chest->toggle();
							break;
						}
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !Settings::is_console_shown)
				{
					player.attack(this->getAbsoluteCursorPosition(), enemy_list);
				}
			}
		}

		if (Settings::is_console_shown) ImGui::SFML::Update(m_window, elapsed_time);

		if (player.isDead())
		{
			if (respawn_timer <= 0.f)
			{
				player.respawn(World::getSpawnPoint());
				respawn_timer = 3.8f;
			}
			else respawn_timer -= tick;
		}

		if (!Settings::is_console_shown) player.control(tick, solid_objects);
		m_camera.setCenter(RoundVector(player.getPosition()));
		//sf::Listener::setPosition(player.getPosition().x, player.getPosition().y, 0);

		World::setCameraRect(this->getCameraRect());
		World::update();

		m_window.setView(m_camera);
		m_window.clear();
		World::render(m_window);

		for (auto it = enemy_list.begin(); it != enemy_list.end(); it++)
		{
			Enemy* cur_it = (Enemy*)*it;
			cur_it->behave(tick, players_list, solid_objects);
		}

		animated_objects.sort([](IAnimated* a, IAnimated* b) {
			return a->getVisibleBounds().top + a->getVisibleBounds().height < b->getVisibleBounds().top + b->getVisibleBounds().height;
		});
		for (auto it = animated_objects.begin(); it != animated_objects.end(); it++)
		{
			(*it)->update(tick);
			m_window.draw(**it);
		}

		this->drawInterface();
		m_window.display();
	}
	m_window.close();
	World::release();
	Animation::release();
	SoundRegister::clear();
	for (auto it = enemy_list.begin(); it != enemy_list.end(); it++)
		delete* it;
}

void Client::drawInterface()
{
	m_window.setView(m_window.getDefaultView());
	// Draw iterface below
	m_window.draw(m_fps);

	if (Settings::is_console_shown)
	{
		this->renderConsole();
		ImGui::SFML::Render(m_window);
	}
	m_window.setView(m_camera);
}

sf::Vector2f Client::getRelativeCursorPosition()
{
	return (sf::Vector2f)sf::Mouse::getPosition(m_window);
}

sf::Vector2f Client::getAbsoluteCursorPosition()
{
	return (sf::Vector2f)sf::Mouse::getPosition(m_window) + m_camera.getCenter() - (m_camera.getSize() / 2.f);
}

sf::IntRect Client::getCameraRect()
{
	return sf::IntRect((sf::Vector2i)(m_camera.getCenter() - (m_camera.getSize() / 2.f)), (sf::Vector2i)m_camera.getSize());
}

void Client::renderConsole()
{
	ImGui::Begin("Console");

	ImGui::BeginChild("Logs", sf::Vector2f(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 25.f), true);
	for (std::string& str : m_console_log)
		ImGui::Text("%s", str.c_str());

	if (m_is_need_resolve)
	{
		ImGui::SetScrollY(ImGui::GetScrollMaxY() + 50.f);
		m_is_need_resolve = false;
	}

	ImGui::EndChild();

	ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
	if (ImGui::InputText("", &m_console_input, ImGuiInputTextFlags_::ImGuiInputTextFlags_EnterReturnsTrue))
	{
		m_console_log.push_back("> " + m_console_input);
		this->handleCommand(m_console_input);
		m_console_input = "";
		m_is_need_resolve = true;
		ImGui::SetKeyboardFocusHere(-1);
	}
	ImGui::PopItemWidth();
	ImGui::End();
}

void Client::handleCommand(std::string line)
{
	
}