#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

struct InfoRow
{
	InfoRow(std::string infoName, std::string infoValue, sf::Color color = sf::Color::Black);
	std::string InfoName;
	std::string InfoValue;
	sf::Color InfoValueColor;
};

class AppInfo
{
	sf::Font m_font;
	sf::Vector2f m_pos;

	sf::Text m_infoNameText;
	sf::Text m_infoValueText;

	std::vector<InfoRow*> m_appData;
	
	InfoRow* m_trustLevelRow;
	InfoRow* m_pathStatusRow;
	InfoRow* m_pathNodeCount;
	InfoRow* m_pathLength;
	InfoRow* m_currentPhase;
	InfoRow* m_pathProgress;

public:
	AppInfo(sf::Vector2f pos);
	void draw(sf::RenderWindow &window);
	
	void updateTrustLevel(const float current, const float treshHold, bool noVal = false);
	void updatePathStatus(bool isFound);
	void updatePathNodeCount(const int current, const int treshhold);
	void updatePathLength(const float length);
	void updateCurrentPhase(const std::string currentPhase, bool end = false);
	void updatePathProgress(const float progress);

	void resetAll();
};

