#include "Button.h"
#include <iostream>

Button::Button(int xPos, int yPos, std::string texturePath, int height)
{
	m_texture.loadFromFile(texturePath);
	m_sprite.setTexture(m_texture);
	m_sprite.setPosition(xPos, yPos);

	m_height = height;
	m_width = m_texture.getSize().x;

	m_sprite.setTextureRect(sf::IntRect(0, 0, m_width, m_height));
}

void Button::update(sf::Vector2f mousePos)
{
	sf::IntRect buttonArea((sf::Vector2i)m_sprite.getPosition(), sf::Vector2i(m_width, m_height));

	if (buttonArea.contains((sf::Vector2i)mousePos) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		m_sprite.setTextureRect(sf::IntRect(0, m_height, m_width, m_height));
		if (m_isPressed == false) {
			m_isPressed = true;
			if(m_callback) m_callback();
		}
	}
	else {
		m_sprite.setTextureRect(sf::IntRect(0, 0, m_width, m_height));
		m_isPressed = false;
	}
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(m_sprite);
}

void Button::setCallback(std::function<void()> callback)
{
	m_callback = callback;
}
