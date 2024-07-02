#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
	sf::Texture m_texture;
	sf::Sprite m_sprite;

	int m_height;
	int m_width;

	bool m_isPressed;
	std::function<void()> m_callback;

public:
	Button(int xPos, int yPos, std::string texturePath, int height);
	void update(sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);
	void setCallback(std::function<void()> callback);
};

