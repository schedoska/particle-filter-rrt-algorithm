#include "Robot.h"
#include "Configuration.h"
#include <iostream>
#include <limits>

#define PI_VALUE 3.14159265359

bool Robot::castRayOnLine(const sf::Vector2f& direction, const Line& obstacle, sf::Vector2f& hitPos)
{
    sf::Vector2f wallStart = obstacle.start;
    sf::Vector2f wallEnd = obstacle.end;

    sf::Vector2f rayStart = m_worldPos;
    sf::Vector2f rayEnd = m_worldPos + direction;

    float A = rayEnd.x - rayStart.x;
    float B = wallEnd.x - wallStart.x;
    float C = wallStart.x - rayStart.x;

    float D = rayEnd.y - rayStart.y;
    float E = wallEnd.y - wallStart.y;
    float F = wallStart.y - rayStart.y;

    float s = (C * E - B * F) / (A * E - B * D);
    float t = -(A * F - C * D) / (A * E - B * D);

    if (t < 0 || t > 1 || s < 0)
        return false;

    hitPos = sf::Vector2f(wallStart.x + t * B, wallStart.y + t * E);
    return true;
}

Line Robot::castRayOnScene(const sf::Vector2f& direction)
{
    Line ray(m_worldPos, m_worldPos);
    float minLen = std::numeric_limits<float>::max();

    std::list<Line>* lineObstacleList = m_scene->getLineObstacles();
    Line line(m_worldPos, m_worldPos);
    for (Line& wall : *lineObstacleList) {
        if (castRayOnLine(direction, wall, line.end)) {
            float len = line.len();
            if (len < minLen) {
                minLen = len;
                ray = line;
            }
        }
    }
    return ray;
}

Robot::Robot(Scene* scene)
{
    m_scene = scene;
    m_direction = 0;
    m_robotShape.setFillColor(sf::Color::Cyan);
    setRadius(10);

    m_scanRayCount = M_ROBOT_RAY_COUNT;
    m_scanList = new Line[m_scanRayCount];
}

void Robot::draw(sf::RenderWindow& window)
{
    m_robotShape.setPosition(m_scene->worldToScreen(m_worldPos));
    window.draw(m_robotShape);
}

void Robot::setRadius(const float radius)
{
    m_robotShape.setRadius(radius);
    m_robotShape.setOrigin(radius, radius);
}

void Robot::setColor(sf::Color color)
{
    m_robotShape.setFillColor(color);
}

void Robot::setDirection(const float direction)
{
    m_direction = direction;
}

float Robot::getDirection() const
{
    return m_direction;
}

void Robot::setPos(const sf::Vector2f pos)
{
    m_worldPos = pos;
}

sf::Vector2f Robot::getPos() const
{
    return m_worldPos;
}

void Robot::scan()
{
    float angleStep = 2.0 * PI_VALUE / m_scanRayCount;
    float dir = m_direction;
    for (int i = 0; i < m_scanRayCount; i++, dir += angleStep) {
        m_scanList[i] = castRayOnScene(sf::Vector2f(sin(dir), -cos(dir)));
    }
}

void Robot::move(float speed, float rotation, float dt)
{
    m_direction += rotation * dt;
    m_worldPos += sf::Vector2f(sin(m_direction) * speed * dt, -cos(m_direction) * speed * dt);
}

void Robot::reset()
{
    for (int i = 0; i < m_scanRayCount; i++) {
        m_scanList[i] = Line();
    }
}
