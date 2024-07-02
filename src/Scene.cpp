#include "Scene.h"

Scene::Scene(sf::Vector2f scenePos)
{
	m_scenePos = scenePos;
	m_scale = 1;

	/*
	m_mapTexture.loadFromFile("C:\\Users\\chedo\\OneDrive\\Pulpit\\PARTICLE_FILTER\\map1.png");
	m_lineObstacles.push_back(Line(sf::Vector2f(12, 8), sf::Vector2f(12, 592)));
	m_lineObstacles.push_back(Line(sf::Vector2f(12, 592), sf::Vector2f(792, 592)));
	m_lineObstacles.push_back(Line(sf::Vector2f(792, 592), sf::Vector2f(792, 12)));
	m_lineObstacles.push_back(Line(sf::Vector2f(792, 12), sf::Vector2f(12, 8)));
	m_lineObstacles.push_back(Line(sf::Vector2f(220, 0), sf::Vector2f(220, 440)));
	m_lineObstacles.push_back(Line(sf::Vector2f(125, 250), sf::Vector2f(220, 250)));
	m_lineObstacles.push_back(Line(sf::Vector2f(222, 250), sf::Vector2f(700, 250)));
	m_lineObstacles.push_back(Line(sf::Vector2f(440, 600), sf::Vector2f(440, 400)));
	m_lineObstacles.push_back(Line(sf::Vector2f(440, 8), sf::Vector2f(440, 90)));
	m_lineObstacles.push_back(Line(sf::Vector2f(440, 200), sf::Vector2f(440, 255)));
	*/

	m_mapTexture.loadFromFile("C:\\Users\\chedo\\OneDrive\\Pulpit\\PARTICLE_FILTER\\map2.png");
	m_lineObstacles.push_back(Line(sf::Vector2f(10, 10), sf::Vector2f(990, 10)));
	m_lineObstacles.push_back(Line(sf::Vector2f(990, 10), sf::Vector2f(990, 690)));
	m_lineObstacles.push_back(Line(sf::Vector2f(990, 690), sf::Vector2f(10, 690)));
	m_lineObstacles.push_back(Line(sf::Vector2f(10, 690), sf::Vector2f(10, 10)));

	m_lineObstacles.push_back(Line(sf::Vector2f(10, 100), sf::Vector2f(100, 100)));
	m_lineObstacles.push_back(Line(sf::Vector2f(266, 10), sf::Vector2f(266, 95)));
	m_lineObstacles.push_back(Line(sf::Vector2f(266, 95), sf::Vector2f(122, 236)));

	m_lineObstacles.push_back(Line(sf::Vector2f(412, 10), sf::Vector2f(412, 50)));
	m_lineObstacles.push_back(Line(sf::Vector2f(604, 10), sf::Vector2f(604, 50)));

	m_lineObstacles.push_back(Line(sf::Vector2f(129, 690), sf::Vector2f(129, 600)));
	m_lineObstacles.push_back(Line(sf::Vector2f(257, 690), sf::Vector2f(257, 560)));

	m_lineObstacles.push_back(Line(sf::Vector2f(129, 500), sf::Vector2f(129, 380)));
	m_lineObstacles.push_back(Line(sf::Vector2f(129, 380), sf::Vector2f(355, 170)));
	m_lineObstacles.push_back(Line(sf::Vector2f(355, 170), sf::Vector2f(413, 170)));
	m_lineObstacles.push_back(Line(sf::Vector2f(410, 145), sf::Vector2f(410, 265)));

	m_lineObstacles.push_back(Line(sf::Vector2f(442, 260), sf::Vector2f(372, 260)));
	m_lineObstacles.push_back(Line(sf::Vector2f(355, 170), sf::Vector2f(413, 170)));
	m_lineObstacles.push_back(Line(sf::Vector2f(384, 263), sf::Vector2f(384, 590)));
	m_lineObstacles.push_back(Line(sf::Vector2f(384, 590), sf::Vector2f(495, 590)));

	m_lineObstacles.push_back(Line(sf::Vector2f(380, 445), sf::Vector2f(328, 445)));
	m_lineObstacles.push_back(Line(sf::Vector2f(128, 445), sf::Vector2f(237, 445)));

	m_lineObstacles.push_back(Line(sf::Vector2f(381, 393), sf::Vector2f(666, 393)));
	m_lineObstacles.push_back(Line(sf::Vector2f(666, 393), sf::Vector2f(666, 259)));
	m_lineObstacles.push_back(Line(sf::Vector2f(666, 259), sf::Vector2f(542, 259)));

	m_lineObstacles.push_back(Line(sf::Vector2f(605, 259), sf::Vector2f(605, 158)));
	m_lineObstacles.push_back(Line(sf::Vector2f(605, 158), sf::Vector2f(861, 158)));
	m_lineObstacles.push_back(Line(sf::Vector2f(861, 104), sf::Vector2f(861, 262)));

	m_lineObstacles.push_back(Line(sf::Vector2f(664, 350), sf::Vector2f(735, 350)));
	m_lineObstacles.push_back(Line(sf::Vector2f(818, 350), sf::Vector2f(990, 350)));

	m_lineObstacles.push_back(Line(sf::Vector2f(607, 690), sf::Vector2f(607, 494)));
	m_lineObstacles.push_back(Line(sf::Vector2f(607, 494), sf::Vector2f(484, 494)));

	m_lineObstacles.push_back(Line(sf::Vector2f(607, 494), sf::Vector2f(738, 494)));
	m_lineObstacles.push_back(Line(sf::Vector2f(738, 494), sf::Vector2f(738, 581)));

	m_lineObstacles.push_back(Line(sf::Vector2f(850, 581), sf::Vector2f(850, 353)));


	m_mapSprite.setTexture(m_mapTexture);
	m_mapSprite.setPosition(scenePos);

	m_drawWalls = true;
	m_wallshape.setFillColor(sf::Color::Blue);
	m_wallshape.setThicness(3);
}

sf::Vector2f Scene::worldToScreen(const sf::Vector2f worldPos) const
{
	return sf::Vector2f(worldPos.x * m_scale, worldPos.y * m_scale) + m_scenePos;
}

sf::Vector2f Scene::screenToWorld(const sf::Vector2f mapPos) const
{
	return sf::Vector2f(mapPos.x / m_scale, mapPos.y / m_scale) - m_scenePos;
}

void Scene::setDrawWalls(bool drawWalls)
{
	m_drawWalls = drawWalls;
}

void Scene::draw(sf::RenderWindow& window)
{
	window.draw(m_mapSprite);

	if (!m_drawWalls) return;
	for (Line& line : m_lineObstacles) {
		m_wallshape.setStartEndPos(worldToScreen(line.start), worldToScreen(line.end));
		window.draw(m_wallshape);
	}
}

std::list<Line>* Scene::getLineObstacles()
{
	return &(m_lineObstacles);
}

sf::Vector2f Scene::getSceneSize() const
{
	return sf::Vector2f(m_mapTexture.getSize().x, m_mapTexture.getSize().y);
}
