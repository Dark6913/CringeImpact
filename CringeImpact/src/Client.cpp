#include "Client.hpp"
#include "Chest.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"

Client::Client()
{
	m_window.create(sf::VideoMode(1600, 800), "Cringe Impact");

	m_world.loadFromFile("data/map/Map.tmx");
	sf::Image cursor_image;
	cursor_image.loadFromFile("data/textures/pointer.png");
	m_cursor.loadFromPixels(cursor_image.getPixelsPtr(), cursor_image.getSize(), sf::Vector2u(0, 0));
	m_window.setMouseCursor(m_cursor);

	m_camera.setSize((sf::Vector2f)m_window.getSize());
	m_camera.setCenter(m_world.getSpawnPoint());

	m_font.loadFromFile("data/fonts/Jost-Regular.ttf");
	m_fps.setFont(m_font);
	m_fps.setFillColor(sf::Color(255, 255, 255));
	m_fps.setPosition(sf::Vector2f(50, 10));
	m_fps.setCharacterSize(20u);
}

void Client::run()
{
	std::list<MapObject*>& nature = m_world.getNatureList();
	std::list<MapObject*>& loot = m_world.getLootList();
	std::list<Solid*> solid_objects;
	std::list<IAnimated*> animated_objects;
	solid_objects.push_back((Solid*)&m_world);
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
	player.setPosition(m_world.getSpawnPoint());
	animated_objects.push_back((IAnimated*)&player);
	solid_objects.push_back((Solid*)&player);

	sf::CircleShape cs;
	cs.setFillColor(sf::Color::Transparent);
	cs.setOutlineColor(sf::Color::Red);
	cs.setRadius(500.f);
	cs.setOrigin(sf::Vector2f(500.f, 500.f));
	cs.setPosition(m_world.getSpawnPoint() + sf::Vector2f(400, 1000));
	cs.setOutlineThickness(1);

	Enemy enemy1;
	enemy1.setPosition(cs.getPosition() + sf::Vector2f(300, 0));
	animated_objects.push_back((IAnimated*)&enemy1);
	solid_objects.push_back((Solid*)&enemy1);

	Enemy enemy2;
	enemy2.setPosition(cs.getPosition() + sf::Vector2f(100, 350));
	animated_objects.push_back((IAnimated*)&enemy2);
	solid_objects.push_back((Solid*)&enemy2);

	Enemy enemy3;
	enemy3.setPosition(cs.getPosition() + sf::Vector2f(-300, -400));
	animated_objects.push_back((IAnimated*)&enemy3);
	solid_objects.push_back((Solid*)&enemy3);

	enemy1.setLivingArea(cs.getPosition(), cs.getRadius());
	enemy2.setLivingArea(cs.getPosition(), cs.getRadius());
	enemy3.setLivingArea(cs.getPosition(), cs.getRadius());

	//sf::Music music;
	//music.openFromFile("data/sound/relax_2.ogg");
	//music.setLoop(true);
	//music.setVolume(20);
	//music.play();

	sf::Shader shader;
	shader.loadFromFile("data/shaders/damaged.frag", sf::Shader::Fragment);

	sf::Clock clock;
	while (m_window.isOpen())
	{
		float tick = clock.restart().asSeconds();
		m_fps.setString("FPS: " + std::to_string((int)round(1.f / tick)));

		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) m_window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) m_window.close();
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
				{
					for (auto it = loot.begin(); it != loot.end(); it++)
					{
						Chest* chest = (Chest*)*it;
						if (VectorModule(chest->getCenter() - player.getPosition()) <= 80.f)
						{
							chest->toggle();
							break;
						}
					}
				}
			}
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					player.attack();
					enemy1.takeDamage(10);
				}
			}
		}

		player.control(tick, this->getAbsoluteCursorPosition(), solid_objects);
		enemy1.behave(tick, solid_objects);
		enemy2.behave(tick, solid_objects);
		enemy3.behave(tick, solid_objects);
		m_camera.setCenter(RoundVector(player.getPosition()));

		m_world.setCameraRect(this->getCameraRect());
		m_world.update();

		m_window.setView(m_camera);
		m_window.clear();
		m_window.draw(m_world);
		m_window.draw(cs);

		for (auto it = loot.begin(); it != loot.end(); it++)
			(*it)->setListenerPosition(player.getPosition());
		enemy1.setListenerPosition(player.getPosition());
		enemy2.setListenerPosition(player.getPosition());
		enemy3.setListenerPosition(player.getPosition());

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
	m_world.release();
	Animation::relese();
}

void Client::drawInterface()
{
	m_window.setView(m_window.getDefaultView());
	// Draw iterface below
	m_window.draw(m_fps);

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