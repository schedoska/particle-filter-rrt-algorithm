#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <list>

class nodeRRT
{
	sf::Vector2f m_pos;
	nodeRRT* m_parentNode;
	float m_cost;

public:
	nodeRRT(sf::Vector2f pos = sf::Vector2f(0, 0), nodeRRT* parent = nullptr);
	sf::Vector2f getPos() const;
	nodeRRT* getParent();
	void setParent(nodeRRT* parent);
	void setCost(const float cost);
	float getCost() const;
};

class RRT
{
	std::list<nodeRRT*> m_nodes;
	Scene* m_scene;

	float m_goalRadius;
	sf::Vector2f m_goalPos;

	nodeRRT* m_bestGoalNode;
	float m_mbestGoalNodeDist;

	float m_stepSize;
	float m_minObstacleDistance;
	float m_neighbourhoodDist;

	sf::CircleShape m_bestGoalNodeShape;
	sf::CircleShape m_nodeShape;
	LineShape m_edgeShape;
	LineShape m_bestPathShape;

	float distanceFromObstacles(sf::Vector2f pos);

	nodeRRT* findClosestNode(sf::Vector2f pos, float& dist);

public:
	RRT(Scene* scene);
	void initNewSearch(sf::Vector2f start, sf::Vector2f goal, float goalRadius);
	void addNewNode();
	void addMultipleNodes(int count);

	void draw(sf::RenderWindow& window);
	std::vector<sf::Vector2f> getPath();
	void reset();

	bool isPathFound();
	int getNodeCount() const;
	float getPathLenght();
};
