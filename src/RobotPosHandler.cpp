#include "RobotPosHandler.h"
#include <iostream>

#define PI_VALUE 3.14159265359

RobotPosHandler::RobotPosHandler(Robot* robot, Scene* scene)
{
	m_robot = robot;
	m_scene = scene;
	m_isPosDragged = false;
	m_isDirectionDragged = false;

	m_directionHandlerShape.setFillColor(sf::Color::Red);
	m_directionHandlerShape.setRadius(10);
	m_directionHandlerShape.setOrigin(10, 10);
	//m_directionHandlerShape.setOutlineColor(sf::Color::Black);
	//m_directionHandlerShape.setOutlineThickness(2);

	m_directionLineShape.setFillColor(sf::Color::Red);
	m_directionLineShape.setThicness(4);
}

void RobotPosHandler::update(sf::Vector2f mousePos)
{
	// ------------------------ DIRECITON HANDLING

	if (Line::len(m_directionHandlerShape.getPosition(), mousePos) < 15 || m_isDirectionDragged) {
		m_directionHandlerShape.setFillColor(sf::Color::Cyan);
		m_directionLineShape.setFillColor(sf::Color::Cyan);
	}
	else {
		m_directionHandlerShape.setFillColor(sf::Color::Red);
		m_directionLineShape.setFillColor(sf::Color::Red);
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_isDirectionDragged == false && m_isPosDragged == false) {
		if (Line::len(m_directionHandlerShape.getPosition(), mousePos) < 15) {
			m_isDirectionDragged = true;
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_isDirectionDragged = false;
	}

	if (m_isDirectionDragged) {
		sf::Vector2f dirVec = mousePos - m_scene->worldToScreen(m_robot->getPos());
		float theta = (-dirVec.y) / Line::len(sf::Vector2f(0, 0), dirVec);
		theta = acos(theta);
		if (mousePos.x < m_scene->worldToScreen(m_robot->getPos()).x) {
			theta = 2.0 * PI_VALUE - theta;
		}
		m_robot->setDirection(theta);
	}

	// ----------------------- POSITION HANLDING

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_isPosDragged == false && m_isDirectionDragged == false) {
		if (Line::len(m_scene->worldToScreen(m_robot->getPos()), mousePos) < 15) {
			m_isPosDragged = true;
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_isPosDragged = false;
	}

	if (m_isPosDragged) {
		m_robot->setPos(m_scene->screenToWorld(mousePos));
	}

	float distance = 60;
	sf::Vector2f handlerPos = sf::Vector2f(sin(m_robot->getDirection()) * distance, -cos(m_robot->getDirection()) * distance);
	handlerPos += m_scene->worldToScreen(m_robot->getPos());
	m_directionHandlerShape.setPosition(handlerPos);
	m_directionLineShape.setStartEndPos(m_scene->worldToScreen(m_robot->getPos()), handlerPos);
}

void RobotPosHandler::draw(sf::RenderWindow& window)
{
	window.draw(m_directionLineShape);
	window.draw(m_directionHandlerShape);
}
