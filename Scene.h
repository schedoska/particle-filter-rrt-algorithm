#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <list>

#include "Line.h"

class Scene
{
	std::list<Line> m_lineObstacles;
	sf::Vector2f m_scenePos;
	float m_scale;
	
	sf::Texture m_mapTexture;
	sf::Sprite m_mapSprite;

	LineShape m_wallshape;
	bool m_drawWalls;

public:
	Scene(sf::Vector2f scenePos);

	sf::Vector2f worldToScreen(const sf::Vector2f worldPos) const;
	sf::Vector2f screenToWorld(const sf::Vector2f screenPos) const;

	void setDrawWalls(bool drawWalls);
	void draw(sf::RenderWindow& window);

	std::list<Line>* getLineObstacles();
	sf::Vector2f getSceneSize() const;
};
