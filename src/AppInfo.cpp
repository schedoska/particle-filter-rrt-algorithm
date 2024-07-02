#include "AppInfo.h"

AppInfo::AppInfo(sf::Vector2f pos)
{
	m_pos = pos;
	m_font.loadFromFile("C:\\Users\\chedo\\OneDrive\\Pulpit\\PARTICLE_FILTER\\OpenSans.ttf");
	
	m_currentPhase = new InfoRow("Current phase:", "Setup", sf::Color::Black);
	m_appData.push_back(m_currentPhase);

	m_trustLevelRow = new InfoRow("Estimation confidence:", " - / -", sf::Color::Red);
	m_appData.push_back(m_trustLevelRow);

	m_pathStatusRow = new InfoRow("RRT* path status:", "-", sf::Color::Red);
	m_appData.push_back(m_pathStatusRow);

	m_pathNodeCount = new InfoRow("RRT* number of nodes:", "- / -", sf::Color::Red);
	m_appData.push_back(m_pathNodeCount);

	m_pathLength = new InfoRow("RRT* path length:", "-", sf::Color::Black);
	m_appData.push_back(m_pathLength);

	m_pathProgress = new InfoRow("Path completion:", "-", sf::Color::Black);
	m_appData.push_back(m_pathProgress);

	m_infoNameText.setFont(m_font);
	m_infoNameText.setFillColor(sf::Color::Black);
	m_infoNameText.setCharacterSize(17);

	m_infoValueText.setFont(m_font);
	m_infoValueText.setFillColor(sf::Color::Black);
	m_infoValueText.setCharacterSize(17);
}

void AppInfo::draw(sf::RenderWindow& window)
{
	int y = 0;
	const int dy = 25;
	const int x = 0;
	const int dx = 200;

	for (InfoRow *rowInfo : m_appData) {
		m_infoNameText.setString(rowInfo->InfoName);
		m_infoValueText.setString(rowInfo->InfoValue);
		m_infoValueText.setFillColor(rowInfo->InfoValueColor);

		m_infoNameText.setPosition(m_pos.x + x, m_pos.y + y);
		m_infoValueText.setPosition(m_pos.x + x + dx, m_pos.y + y);
		y += dy;

		window.draw(m_infoNameText);
		window.draw(m_infoValueText);
	}
}

void AppInfo::updateTrustLevel(float current, float treshHold, bool noVal)
{
	if (noVal) {
		m_trustLevelRow->InfoValueColor = sf::Color::Red;
		m_trustLevelRow->InfoValue = "- / -";
		return;
	}

	if(current > treshHold)
		m_trustLevelRow->InfoValueColor = sf::Color(0, 196, 10);

	std::stringstream ss_1, ss_2;
	ss_1 << std::fixed << std::setprecision(1) << current;
	std::string val_1 = ss_1.str();

	ss_2 << std::fixed << std::setprecision(1) << treshHold;
	std::string val_2 = ss_2.str();
		
	m_trustLevelRow->InfoValue = val_1 + " / " + val_2;
}

void AppInfo::updatePathStatus(bool isFound)
{
	if (isFound) {
		m_pathStatusRow->InfoValue = "PATH FOUND";
		m_pathStatusRow->InfoValueColor = sf::Color(0, 196, 10);
	}
	else {
		m_pathStatusRow->InfoValue = "NOT FOUND";
		m_pathStatusRow->InfoValueColor = sf::Color::Red;
	}
}

void AppInfo::updatePathNodeCount(const int current, const int treshhold)
{
	m_pathNodeCount->InfoValue = std::to_string(current) + " / " + std::to_string(treshhold);
	if(current >= treshhold) 
		m_pathNodeCount->InfoValueColor = sf::Color(0, 196, 10);
}

void AppInfo::updatePathLength(const float length)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << length;
	std::string val = ss.str();

	m_pathLength->InfoValue = val;
}

void AppInfo::updateCurrentPhase(const std::string currentPhase, bool end)
{
	m_currentPhase->InfoValue = currentPhase;
	if(end) m_currentPhase->InfoValueColor = sf::Color(0, 196, 10);
	else m_currentPhase->InfoValueColor = sf::Color::Black;
}

void AppInfo::updatePathProgress(const float progress)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << progress;
	std::string val = ss.str();

	m_pathProgress->InfoValue = val + " %";
	if (progress >= 99.99) {
		m_pathProgress->InfoValueColor = sf::Color(0, 196, 10);
	}
}

void AppInfo::resetAll()
{
	m_trustLevelRow->InfoValueColor = sf::Color::Red;
	m_trustLevelRow->InfoValue = "- / -";

	m_pathStatusRow->InfoValue = "-";
	m_pathStatusRow->InfoValueColor = sf::Color::Red;

	m_pathNodeCount->InfoValue = "- / -";
	m_pathNodeCount->InfoValueColor = sf::Color::Red;

	m_pathLength->InfoValue = "-";
	m_pathLength->InfoValueColor = sf::Color::Black;

	m_currentPhase->InfoValue = "SETUP";
	m_currentPhase->InfoValueColor = sf::Color::Black;

	m_pathProgress->InfoValue = "-";
	m_pathProgress->InfoValueColor = sf::Color::Black;
}

InfoRow::InfoRow(std::string infoName, std::string infoValue, sf::Color color)
{
	InfoName = infoName;
	InfoValue = infoValue;
	InfoValueColor = color;
}
