#include "ParticleFilter.h"
#include <cstdlib>
#include <iostream> 
#include <limits>
#include <algorithm>
#include "Configuration.h"

ParticleFilter::ParticleFilter(Scene* scene)
{
	srand((unsigned)time(NULL));
	m_scene = scene;
	m_sceneSize = (sf::Vector2i)scene->getSceneSize();

	const int sampleCount = M_PARTICLE_FILTER_SAMPLE_COUNT;
	for (int i = 0; i < sampleCount; i++) {
		SampleRobot robot(scene);
		robot.setPos(sf::Vector2f(rand() % m_sceneSize.x, rand() % m_sceneSize.y));
		robot.setDirection((rand() % (2 * 314)) / 100.0);
		m_sampleList.push_back(robot);
	}

	m_estimatedPosRobot = new SampleRobot(scene);
	m_estimatedPosRobot->setColor(sf::Color::Transparent);
	m_estimatedPosRobot->setRadius(30);
	m_estimatedPosRobot->getRobotShape()->setOutlineColor(sf::Color::Yellow);
	m_estimatedPosRobot->getRobotShape()->setOutlineThickness(5);

	m_trustLevel = std::numeric_limits<float>::max();
}

void ParticleFilter::draw(sf::RenderWindow& window)
{
	for (SampleRobot& robot : m_sampleList) {
		robot.draw(window);
	}
	m_estimatedPosRobot->draw(window);
}

void ParticleFilter::updateParticles(float speed, float rotation, float dt)
{
	for (SampleRobot& robot : m_sampleList) {
		robot.move(speed, rotation, dt);
	}
	m_estimatedPosRobot->move(speed, rotation, dt);
}

void ParticleFilter::calculateMatchScores(Line* scanList)
{
	for (SampleRobot& robot : m_sampleList) {
		robot.scanAndCompare(scanList);
	}
}

void ParticleFilter::resample()
{
	std::sort(m_sampleList.begin(), m_sampleList.end(), SampleRobotComparator);
	m_estimatedPosRobot->setPos(m_sampleList[0].getPos());
	m_estimatedPosRobot->setDirection(m_sampleList[0].getDirection());
	m_estimatedPos = m_sampleList[0].getPos();

	float trustDistance = 70;
	int trustCount = 0;

	/*int h = m_sampleList.size() / 2;
	int spreadArea = 50;
	float spreadAngle = 0.7;

	for (int i = 0; i < h; i++) {
		if (Line::len(m_sampleList[i].getPos(), m_estimatedPos) < trustDistance)
			trustCount++;

		float ranDir = (rand() % 6283) / 1000.0;
		float ranSpread = (rand() % 500) / 10.0;
		sf::Vector2f newPos = sf::Vector2f(sin(ranDir) * ranSpread, -cos(ranDir) * ranSpread);
		m_sampleList[h + i].setPos(m_sampleList[i].getPos() + newPos);
		m_sampleList[h + i].setDirection(m_sampleList[i].getDirection() +
			(rand() % (2 * (int)(spreadAngle*1000)) - (int)(spreadAngle * 1000)/1000.0));
	}*/

	int h = m_sampleList.size() / 4;
	float spreadAngle = 0.7;
	for (int i = 0; i < h; i++) {
		if (Line::len(m_sampleList[i].getPos(), m_estimatedPos) < trustDistance)
			trustCount++;

		for (int q = 0; q < 3; q++) {
			float ranDir = (rand() % 6283) / 1000.0;
			float ranSpread = (rand() % 500) / 10.0;
			sf::Vector2f newPos = sf::Vector2f(sin(ranDir) * ranSpread, -cos(ranDir) * ranSpread);
			m_sampleList[h + i + q * h].setPos(m_sampleList[i].getPos() + newPos);
			m_sampleList[h + i + q * h].setDirection(m_sampleList[i].getDirection() +
				(rand() % (2 * (int)(spreadAngle * 1000)) - (int)(spreadAngle * 1000) / 1000.0));
		}
	}


	m_trustLevel = (float)trustCount / (float)h;
}

sf::Vector2f ParticleFilter::getEstimatedPos() const
{
	return m_estimatedPos;
}

float ParticleFilter::getTrustLevel() const
{
	if (m_trustLevel > 1.0) return 0;
	return m_trustLevel;
}

void ParticleFilter::reset()
{
	srand((unsigned)time(NULL));

	const int sampleCount = M_PARTICLE_FILTER_SAMPLE_COUNT;
	for (int i = 0; i < sampleCount; i++) {
		m_sampleList[i].setPos(sf::Vector2f(rand() % m_sceneSize.x, rand() % m_sceneSize.y));
		m_sampleList[i].setDirection((rand() % (2 * 314)) / 100.0);
	}

	m_trustLevel = std::numeric_limits<float>::max();
}

