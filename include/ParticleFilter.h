#pragma once

#include "Scene.h"
#include "SampleRobot.h"

class ParticleFilter
{
	Scene* m_scene;
	std::vector<SampleRobot> m_sampleList;
	sf::Vector2f m_estimatedPos;
	SampleRobot* m_estimatedPosRobot;
	sf::Vector2i m_sceneSize;

	float m_trustLevel; // low level = good;

public:
	ParticleFilter(Scene* scene);
	void draw(sf::RenderWindow& window);
	void updateParticles(float speed, float rotation, float dt = 1);
	void calculateMatchScores(Line *scanList);
	void resample();

	sf::Vector2f getEstimatedPos() const;
	float getTrustLevel() const;

	void reset();
};
