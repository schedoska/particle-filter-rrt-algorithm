#include "GoalPosHandler.h"
#include "Line.h"

GoalPosHandler::GoalPosHandler(sf::CircleShape* goalPosShape)
{
	m_goalPosShape = goalPosShape;
	m_goalRadius = 50;

	m_goalRadiusShape.setFillColor(sf::Color::Transparent);
	m_goalRadiusShape.setOutlineColor(sf::Color(45, 56, 46));
	m_goalRadiusShape.setOutlineThickness(4);
	m_goalRadiusShape.setRadius(m_goalRadius);
	m_goalRadiusShape.setOrigin(m_goalRadius, m_goalRadius);

	m_goalRadiusShape.setPosition(m_goalPosShape->getPosition());
	m_goalRadiusShape.setPointCount(60);

	m_isPosDragged = false;
	m_isRadiusDragged = false;
}

void GoalPosHandler::update(sf::Vector2f mousePos)
{
	/*/float mouseGoalDist = Line::len(mousePos, m_goalPosShape->getPosition());
	float hitThicness = 7;
	if (mouseGoalDist > m_goalRadius - hitThicness && mouseGoalDist < m_goalRadius + hitThicness) {
		m_goalRadiusShape.setOutlineColor(sf::Color::Cyan);
	}
	else {
		m_goalRadiusShape.setOutlineColor(sf::Color::Black);
	}*/

	// ---------------------------- POSITION HANDLING

	float mouseGoalDist = Line::len(mousePos, m_goalPosShape->getPosition());

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_isPosDragged == false && m_isRadiusDragged == false) {
		if (mouseGoalDist < 15) {
			m_isPosDragged = true;
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_isPosDragged = false;
	}

	if (m_isPosDragged) {
		m_goalPosShape->setPosition(mousePos);
	}

	// --------------------------- RADIUS HANDLING

	float hitThicness = 7;
	if ((mouseGoalDist > m_goalRadius - hitThicness && mouseGoalDist < m_goalRadius + hitThicness) || m_isRadiusDragged) {
		m_goalRadiusShape.setOutlineColor(sf::Color::Cyan);
	}
	else {
		m_goalRadiusShape.setOutlineColor(sf::Color(1, 52, 145));
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_isPosDragged == false && m_isRadiusDragged == false) {
		if (mouseGoalDist > m_goalRadius - hitThicness && mouseGoalDist < m_goalRadius + hitThicness) {
			m_isRadiusDragged = true;
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		m_isRadiusDragged = false;
	}

	if (m_isRadiusDragged) {
		m_goalRadius = mouseGoalDist;
	}

	m_goalRadiusShape.setPosition(m_goalPosShape->getPosition());
	m_goalRadiusShape.setRadius(m_goalRadius);
	m_goalRadiusShape.setOrigin(m_goalRadius, m_goalRadius);
}

void GoalPosHandler::draw(sf::RenderWindow& window)
{
	window.draw(m_goalRadiusShape);
}

float GoalPosHandler::getGoalRadius() const
{
	return m_goalRadius;
}


