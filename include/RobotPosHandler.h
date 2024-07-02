#pragma once

#include "Robot.h"

class RobotPosHandler
{
	Robot* m_robot;
	Scene* m_scene;
	bool m_isPosDragged;
	bool m_isDirectionDragged;

	sf::CircleShape m_directionHandlerShape;
	LineShape m_directionLineShape;

public:
	RobotPosHandler(Robot *robot, Scene *scene);
	void update(sf::Vector2f mousePos);
	void draw(sf::RenderWindow &window);
};

