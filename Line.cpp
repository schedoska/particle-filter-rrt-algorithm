#include "Line.h"

#define PI_VAL 3.14159265358979323846

void LineShape::setLine()
{
    float dx = getPosition().x - m_endPos.x;
    float dy = getPosition().y - m_endPos.y;
    float len = sqrt(pow(dx, 2) + pow(dy, 2));
    setSize(sf::Vector2f(len, m_thicness));

    if (dx == 0) dy *= -1;
    setRotation(atan(dy / dx) * 180.0 / PI_VAL);
    if (dx > 0) rotate(180.0);
    setOrigin(0, m_thicness / 2.0);
}

LineShape::LineShape(sf::Vector2f start, sf::Vector2f end)
{
    m_thicness = 3;
    setFillColor(sf::Color::Red);
    setPosition(start);
    setEndPos(end);
    setLine();
}

void LineShape::setStartPos(const sf::Vector2f startPos)
{
    setPosition(startPos);
    setLine();
}

void LineShape::setEndPos(const sf::Vector2f endPos)
{
    m_endPos = endPos;
    setLine();
}

void LineShape::setStartEndPos(const sf::Vector2f startPos, const sf::Vector2f endPos)
{
    setStartPos(startPos);
    setEndPos(endPos);
    setLine();
}

void LineShape::setThicness(const float val)
{
    m_thicness = val;
    setLine();
}

sf::Vector2f LineShape::getStartPos() const
{
    return getPosition();
}

sf::Vector2f LineShape::getEndPos() const
{
    return m_endPos;
}

Line::Line(sf::Vector2f start, sf::Vector2f end)
{
    this->start = start;
    this->end = end;
}

float Line::len() const
{
    return sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2));
}

float Line::len(sf::Vector2f start, sf::Vector2f end)
{
    return sqrt(pow(start.x - end.x, 2) + pow(start.y - end.y, 2));
}


