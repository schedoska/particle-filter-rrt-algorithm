#include "SampleRobot.h"
#include <iostream>

SampleRobot::SampleRobot(Scene* scene)
	: Robot(scene)
{
	setColor(sf::Color(255, 125, 25, 40));
	setRadius(4);
}

void SampleRobot::scanAndCompare(Line* scanList)
{
	m_matchScore = 0;
	scan();
	for (int i = 0; i < m_scanRayCount; i++) {
		m_matchScore += abs(m_scanList[i].len() - scanList[i].len());
	}
}

float SampleRobot::getMatchScore() const
{
	return m_matchScore;
}

sf::CircleShape* SampleRobot::getRobotShape()
{
	return &m_robotShape;
}

bool SampleRobotComparator(const SampleRobot& a, const SampleRobot& b)
{
	return (a.getMatchScore() < b.getMatchScore());
}
