#pragma once

#include "Robot.h"

class SampleRobot : public Robot
{
	float m_matchScore;

public:
	SampleRobot(Scene *scene);
	void scanAndCompare(Line* scanList);
	float getMatchScore() const;

	sf::CircleShape* getRobotShape();
};

bool SampleRobotComparator(const SampleRobot& a, const SampleRobot& b);