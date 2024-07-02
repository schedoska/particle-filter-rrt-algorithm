#include "RRT.h"
#include "Configuration.h"
#include <iostream>

float RRT::distanceFromObstacles(sf::Vector2f pos)
{
	std::list<Line>* walls = m_scene->getLineObstacles();

	float minDist = std::numeric_limits<float>::max();
	for (Line& line : *walls) {
		sf::Vector2f AB = line.end - line.start;
		sf::Vector2f AC = pos - line.start;
		sf::Vector2f BA = line.start - line.end;
		sf::Vector2f BC = pos - line.end;

		float dotAB_AC = AB.x * AC.x + AB.y * AC.y;
		if (dotAB_AC < 0) {
			minDist = std::min(minDist, Line::len(line.start, pos));
			continue;
		}
		float dotBA_BC = BA.x * BC.x + BA.y * BC.y;
		if (dotBA_BC < 0) {
			minDist = std::min(minDist, Line::len(line.end, pos));
			continue;
		}

		float area = abs(AC.x * AB.y - AC.y * AB.x);
		minDist = std::min(minDist, area / Line::len(line.start, line.end));
	}

	return minDist;
}

nodeRRT* RRT::findClosestNode(sf::Vector2f pos, float& dist)
{
	float minDist = std::numeric_limits<float>::max();
	nodeRRT* closestNode = nullptr;

	for (nodeRRT* node : m_nodes) {
		float dist = Line::len(node->getPos(), pos);
		if (dist < minDist) {
			minDist = dist;
			closestNode = node;
		}
	}
	dist = minDist;
	return closestNode;
}

RRT::RRT(Scene* scene)
{
	m_scene = scene;
	m_edgeShape.setFillColor(sf::Color::Blue);
	m_edgeShape.setThicness(2);

	m_nodeShape.setFillColor(sf::Color::Green);
	m_nodeShape.setRadius(5);
	m_nodeShape.setOrigin(5,5);
	
	m_bestGoalNodeShape.setFillColor(sf::Color::Red);
	m_bestGoalNodeShape.setRadius(10);
	m_bestGoalNodeShape.setOrigin(10, 10);

	m_bestPathShape.setFillColor(sf::Color::Red);
	m_bestPathShape.setThicness(6);

	m_stepSize = M_RRT_STEP_SIZE;
	m_minObstacleDistance = M_RRT_MIN_OBSTACLE_DISTANCE;
	m_neighbourhoodDist = M_RRT_NEIGHBOURHOOD_DIST;
	m_bestGoalNode = nullptr;
}

void RRT::initNewSearch(sf::Vector2f start, sf::Vector2f goal, float goalRadius)
{
	m_goalRadius = goalRadius;
	m_goalPos = goal;

	nodeRRT* node = new nodeRRT(start);
	m_nodes.push_back(node);

	m_mbestGoalNodeDist = std::numeric_limits<float>::max();
	m_bestGoalNode = nullptr;
}

void RRT::addNewNode()
{
	sf::Vector2i sceneSize = (sf::Vector2i)m_scene->getSceneSize();
	sf::Vector2f newNodePos;

	do {
		sf::Vector2f sample = sf::Vector2f(rand() % sceneSize.x, rand() % sceneSize.y);
		float dist = 0;
		nodeRRT* closestNode = findClosestNode(sample, dist);

		if (dist < m_stepSize) {
			newNodePos = sample;
		}
		else {
			sf::Vector2f closestToSample = sample - closestNode->getPos();
			newNodePos = sf::Vector2f(closestToSample.x * m_stepSize / dist, closestToSample.y * m_stepSize / dist);
			newNodePos += closestNode->getPos();
		}
	} while (distanceFromObstacles(newNodePos) < m_minObstacleDistance); 

	float minCost = std::numeric_limits<float>::max();
	nodeRRT* parentNode = nullptr;

	std::list<nodeRRT*> neighbourNodes;
	for (nodeRRT* node : m_nodes) {
		if (Line::len(newNodePos, node->getPos()) < m_neighbourhoodDist) {
			float pc = Line::len(newNodePos, node->getPos()) + node->getCost();
			if (pc < minCost) {
				minCost = pc;
				parentNode = node;
			}
			else {
				neighbourNodes.push_back(node);
			}
		}
	}
	nodeRRT* newNode = new nodeRRT(newNodePos, parentNode);
	newNode->setCost(minCost);
	m_nodes.push_back(newNode);

	for (nodeRRT* node : neighbourNodes) {
		float pc = Line::len(node->getPos(), newNodePos) + minCost;
		if (pc < node->getCost()) {
			node->setParent(newNode);
			node->setCost(pc);
		}
	}
	
	float goalDist = Line::len(newNodePos, m_goalPos);
	if (goalDist < m_goalRadius && goalDist < m_mbestGoalNodeDist) {
		m_mbestGoalNodeDist = goalDist;
		m_bestGoalNode = newNode;
	}
}

void RRT::addMultipleNodes(int count)
{
	if (count < 1) return;
	for (int i = 0; i < count; i++) {
		addNewNode();
	}
}

void RRT::draw(sf::RenderWindow& window)
{
	for (nodeRRT* node : m_nodes) {
		if (node->getParent() != nullptr) {
			m_edgeShape.setStartEndPos(m_scene->worldToScreen(node->getPos()), m_scene->worldToScreen(node->getParent()->getPos()));
			window.draw(m_edgeShape);
		}
		m_nodeShape.setPosition(m_scene->worldToScreen(node->getPos()));
		window.draw(m_nodeShape);
	}
	if (m_bestGoalNode != nullptr) {
		m_bestGoalNodeShape.setPosition(m_scene->worldToScreen(m_bestGoalNode->getPos()));
		window.draw(m_bestGoalNodeShape);

		nodeRRT* current = m_bestGoalNode;
		while (current->getParent() != nullptr) {
			m_bestPathShape.setStartEndPos(
				m_scene->worldToScreen(current->getPos()), m_scene->worldToScreen(current->getParent()->getPos()));
			window.draw(m_bestPathShape);
			current = current->getParent();
		}
	}
}

std::vector<sf::Vector2f> RRT::getPath()
{
	std::vector<sf::Vector2f> list;
	nodeRRT* current = m_bestGoalNode;
	while (current->getParent() != nullptr) {
		list.push_back(current->getPos());
		current = current->getParent();
	}
	return list;
}

void RRT::reset()
{
	for (nodeRRT* node : m_nodes) {
		delete node;
	}
	m_nodes.clear();
	m_bestGoalNode = nullptr;
}

bool RRT::isPathFound()
{
	return m_bestGoalNode == nullptr ? false : true;
}

int RRT::getNodeCount() const
{
	return m_nodes.size();
}

float RRT::getPathLenght()
{
	if (isPathFound()) {
		float len = 0;
		nodeRRT* current = m_bestGoalNode;
		while (current->getParent() != nullptr) {
			len += Line::len(current->getPos(), current->getParent()->getPos());
			current = current->getParent();
		}
		return len;
	}
	else {
		return 0;
	}
}

nodeRRT::nodeRRT(sf::Vector2f pos, nodeRRT* parent)
{
	m_pos = pos;
	m_parentNode = parent;
	m_cost = 0;
}

sf::Vector2f nodeRRT::getPos() const
{
	return m_pos;
}

nodeRRT* nodeRRT::getParent()
{
	return m_parentNode;
}

void nodeRRT::setParent(nodeRRT* parent)
{
	m_parentNode = parent;
}

void nodeRRT::setCost(const float cost)
{
	m_cost = cost;
}

float nodeRRT::getCost() const
{
	return m_cost;
}

