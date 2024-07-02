#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

struct Line
{
	Line(sf::Vector2f start = sf::Vector2f(0,0), sf::Vector2f end = sf::Vector2f(0, 0));
	sf::Vector2f start;
	sf::Vector2f end;
	float len() const;
	static float len(sf::Vector2f start, sf::Vector2f end);
};

class LineShape : public sf::RectangleShape
{
	sf::Vector2f m_endPos;
	float m_thicness;
	void setLine();

public:
	LineShape(sf::Vector2f start = sf::Vector2f(0,0), sf::Vector2f end = sf::Vector2f(0,0));
	void setStartPos(const sf::Vector2f startPos);
	void setEndPos(const sf::Vector2f endPos);
	void setStartEndPos(const sf::Vector2f startPos, const sf::Vector2f endPos);
	void setThicness(const float val);

	sf::Vector2f getStartPos() const;
	sf::Vector2f getEndPos() const;
};

