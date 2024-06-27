#include "ControlRobot.h"
#include <iostream>

#define PI_VALUE 3.14159265359

void ControlRobot::exploteUpdate(float dt)
{
	scan();

	if (m_direction < m_goalDirection - 0.1) {
		move(0, 2, dt);
		//m_particleFilter->updateParticles(0, 2 + ((rand() % 20) - 10) / 100.0, dt);
		m_particleFilter->updateParticles(0, 2, dt);
	}
	else if (m_direction > m_goalDirection + 0.1) {
		move(0, -2, dt);
		//m_particleFilter->updateParticles(0, -2 + ((rand() % 20) - 10) / 100.0, dt);
		m_particleFilter->updateParticles(0, -2, dt);
	}
	else {
		if (m_scanList[1].len() < 50) {
			m_goalDirection = m_direction - (rand() % 10) / 10.0;
		}
		else if (m_scanList[m_scanRayCount - 1].len() < 50) {
			m_goalDirection = m_direction + (rand() % 10) / 10.0;
		}
		else {
			move(50, 0, dt);
			//m_particleFilter->updateParticles(50 + ((rand() % 6) - 3), 0, dt);
			m_particleFilter->updateParticles(50, 0, dt);
		}
	}

	static float time = 0;
	time += dt;

	if (time > 0.5) {
		m_particleFilter->calculateMatchScores(m_scanList);
		m_particleFilter->resample();
		time = 0;
	}
}

void ControlRobot::pathUpdate(float dt)
{
	scan();
	static float time = 0;
	time += dt;
	if (time > 0.5) {
		m_particleFilter->calculateMatchScores(m_scanList);
		m_particleFilter->resample();
		time = 0;
	}

	sf::Vector2f estPos = getEstimatedPos();

	if (Line::len(m_path[m_pathStep], estPos) < 30) {
		m_pathStep--;
		if (m_pathStep < 0) {
			m_robotState = RobotState::Idle;
			return;
		}
	}

	sf::Vector2f dirVec = m_path[m_pathStep] - estPos;
	float theta = (-dirVec.y) / Line::len(sf::Vector2f(0, 0), dirVec);
	theta = acos(theta);
	if (m_path[m_pathStep].x < getPos().x) {
		theta = 2.0 * PI_VALUE - theta;
	}

	float deltaTheta = theta - normalizeAngle(m_direction);
	deltaTheta = normalizeAngle(deltaTheta);
	if (deltaTheta < 0.1 && deltaTheta > -0.1)
	{
		move(50, 0, dt);
		m_particleFilter->updateParticles(50, 0, dt);
	}
	else if (deltaTheta > 0 && deltaTheta < PI_VALUE) {
		move(50, 2, dt);
		m_particleFilter->updateParticles(50, 2, dt);
	}
	else {
		move(50, -2, dt);
		m_particleFilter->updateParticles(50, -2, dt);
	}
}

ControlRobot::ControlRobot(Scene* scene)
	:Robot(scene)
{
	m_scanRayShape.setFillColor(sf::Color::Red);
	m_scanRayShape.setThicness(2);
	setRadius(20);

	m_directionShape.setFillColor(sf::Color::Black);
	m_directionShape.setThicness(4);

	m_pathLineShape.setFillColor(sf::Color::Green);
	m_pathLineShape.setThicness(6);

	m_particleFilter = new ParticleFilter(scene);
	m_goalDirection = m_direction;

	m_robotState = RobotState::Explore;
	m_drawParticleFilter = false;
}

void ControlRobot::update(float dt)
{
	switch (m_robotState) {
		case RobotState::Explore:
			exploteUpdate(dt);
			break;
		case RobotState::Path:
			pathUpdate(dt);
			break;
		default:
			break;
	}
}

void ControlRobot::draw(sf::RenderWindow& window)
{
	if(m_drawParticleFilter)
		m_particleFilter->draw(window);

	if (m_path.size() > 2) {
		for (int i = 0; i < m_path.size() - 1; i++) {
			if (i < m_pathStep + 1) {
				m_pathLineShape.setFillColor(sf::Color::Blue);
			}
			else {
				m_pathLineShape.setFillColor(sf::Color::Green);
			}
			m_pathLineShape.setStartEndPos(m_scene->worldToScreen(m_path[i]), 
				m_scene->worldToScreen(m_path[i + 1]));
			window.draw(m_pathLineShape);
		}
	}

	Robot::draw(window);
	sf::Vector2f robotScreenPos = m_scene->worldToScreen(getPos());
	m_directionShape.setStartEndPos(robotScreenPos, robotScreenPos + sf::Vector2f(sin(m_direction)*20, -cos(m_direction)*20));
	window.draw(m_directionShape);
}

void ControlRobot::drawScanLines(sf::RenderWindow& window)
{
	for (int i = 0; i < m_scanRayCount; i++) {
		sf::Vector2f rayStart = m_scene->worldToScreen(m_scanList[i].start);
		sf::Vector2f rayEnd = m_scene->worldToScreen(m_scanList[i].end);
		m_scanRayShape.setStartEndPos(rayStart, rayEnd);
		window.draw(m_scanRayShape);
	}
}

sf::Vector2f ControlRobot::getEstimatedPos()
{
	return m_particleFilter->getEstimatedPos();
}

void ControlRobot::goPath(std::vector<sf::Vector2f> path)
{
	m_path = path;
	m_robotState = RobotState::Path;
	m_pathStep = path.size() - 1;
}

float ControlRobot::getEstimationTrust()
{
	return m_particleFilter->getTrustLevel();
}

void ControlRobot::reset()
{
	Robot::reset();
	m_particleFilter->reset();
	m_robotState = RobotState::Explore;
	m_path.clear();
}

void ControlRobot::setDirection(float direction)
{
	Robot::setDirection(direction);
	m_goalDirection = direction;
}

void ControlRobot::setDrawParticleFilter(bool draw)
{
	m_drawParticleFilter = draw;
}

bool ControlRobot::isIdle() const
{
	return m_robotState == RobotState::Idle ? true : false;
}

float ControlRobot::getPathCompletion()
{
	return (m_path.size() -  m_pathStep - 1) * 100.0 / (float)m_path.size();
}

float normalizeAngle(float rad)
{
	if (rad >= 2.0 * PI_VALUE) {
		return (rad - 2.0 * PI_VALUE);
	}
	if (rad < 0) {
		return (rad + 2.0 * PI_VALUE);
	}
	return rad;
}
