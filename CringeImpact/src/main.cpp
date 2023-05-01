#include <SFML/Graphics.hpp>
#include "VectorMath.hpp"

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(1600, 800), "Window");

    sf::Vector2f center = {800, 400};
    float radius = 150.f;

    const int DOTS_COUNT = 30;
    sf::VertexArray circle;
    circle.setPrimitiveType(sf::LineStrip);
    circle.resize(DOTS_COUNT);
    float delta_angle = 2.f * M_PI / (float)DOTS_COUNT;
    for (int i = 0; i < DOTS_COUNT; i++)
    {
        sf::Vertex vertex;
        vertex.color = sf::Color::Red;
        vertex.position.x = center.x + radius * cos(i * delta_angle);
        vertex.position.y = center.y + radius * sin(i * delta_angle);
        circle.append(vertex);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear();
        window.draw(circle);
        window.display();
    }

    return 0;
}