#pragma once

#include "Robot.h"
#include "ParticleFilter.h"
#include <cmath>

enum class RobotState {Explore, Path, Idle};

class ControlRobot : public Robot
{
	RobotState m_robotState;
	void exploteUpdate(float dt);
	void pathUpdate(float dt);
	//void IdleUpdate();

	LineShape m_scanRayShape;
	LineShape m_directionShape;
	ParticleFilter *m_particleFilter;

	std::vector<sf::Vector2f> m_path;
	int m_pathStep;
	LineShape m_pathLineShape;

	float m_goalDirection;
	bool m_drawParticleFilter;

public:
	ControlRobot(Scene* scene);
	void update(float dt);
	void draw(sf::RenderWindow& window) override;
	void drawScanLines(sf::RenderWindow& window);
	sf::Vector2f getEstimatedPos();

	void goPath(std::vector<sf::Vector2f> path);
	float getEstimationTrust();

	void reset() override;
	void setDirection(float direction) override;

	void setDrawParticleFilter(bool draw);
	bool isIdle() const;
	float getPathCompletion();
};

float normalizeAngle(float rad);