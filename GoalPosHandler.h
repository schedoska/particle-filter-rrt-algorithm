#pragma once

#include <SFML/Graphics.hpp>
#include "Scene.h"

class GoalPosHandler
{
	sf::CircleShape* m_goalPosShape;
	sf::CircleShape m_goalRadiusShape;
	float m_goalRadius;

	bool m_isPosDragged;
	bool m_isRadiusDragged;

public:
	GoalPosHandler(sf::CircleShape* goalPosShape);
	void update(sf::Vector2f mousePos);
	void draw(sf::RenderWindow& window);

	float getGoalRadius() const;
};

