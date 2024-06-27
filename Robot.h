#pragma once

#include <SFML/Graphics.hpp>
#include "Scene.h"

class Robot
{
protected:
	sf::Vector2f m_worldPos;
	float m_direction; // radians

	Scene* m_scene;
	Line* m_scanList;
	int m_scanRayCount;

	sf::CircleShape m_robotShape;

	bool castRayOnLine(const sf::Vector2f& direction, const Line& obstacle, sf::Vector2f& hitPos);
	Line castRayOnScene(const sf::Vector2f& direction);

public:
	Robot(Scene* scene);
	virtual void draw(sf::RenderWindow &window);
	void setRadius(const float radius);
	void setColor(sf::Color color);

	virtual void setDirection(const float direction);
	float getDirection() const;
	void setPos(const sf::Vector2f pos);
	sf::Vector2f getPos() const;
	void scan();
	void move(float speed, float rotation, float dt = 1);

	virtual void reset();
};


/*
#define SCAN_RAY_COUNT 8

class Robot
{
	float m_direction;	// Radians
	sf::Vector2f m_worldPos;
	Scene* m_scene;

	float m_scanRange;
	float m_raySpreadAngle; // Radians
	Line m_rayScanList[SCAN_RAY_COUNT];

public:
	sf::CircleShape m_robotBody;

	Robot(Scene *scene);
	void draw(sf::RenderWindow &window);
	void setPos(sf::Vector2f pos);
	sf::Vector2f getPos();
	void setDir(float dir);
	float getDir();
	void moveForward(float val);
	void rotate(float val);

	bool castRayOnLine(const sf::Vector2f& direction, const Line& obstacle, sf::Vector2f &hitPos);
	Line castRayOnScene(const sf::Vector2f& direction);
	void scan();
	Line* getRayScanList();

	void setRadius(float radius);
};

struct OdometryData
{
	OdometryData(float distance = 0, float rotation = 0);
	void clear();
	float distance;
	float rotation;
};

*/