#include "Application.h"
#include <iostream>

void Application::setupStateUpdate(float dt)
{
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(*m_window);
	m_robotPosHandler->update(mousePos);
	m_goalPosHandler->update(mousePos);
}

void Application::estimationStateUpdate(float dt)
{
	m_robot->update(dt);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) || m_robot->getEstimationTrust() > 0.7) {
		m_applicationState = ApplicationState::PathFinding;
		m_RRT->initNewSearch(m_robot->getEstimatedPos(), 
			m_scene->screenToWorld(m_goalMarkerShape.getPosition()), 
			m_goalPosHandler->getGoalRadius());
		m_rrtTime = 0;
		m_appInfo->updatePathStatus(false);
		m_appInfo->updateCurrentPhase("PATH FINDING");

		m_robot->setDrawParticleFilter(false);
	}

	m_appInfo->updateTrustLevel(m_robot->getEstimationTrust() * 100.0, 0.7 * 100.0);
}

void Application::pathFindingStateUpdate(float dt)
{
	const int maxNodeCount = 600;
	m_rrtTime += dt;
	if (m_rrtTime > 0.1) {
		m_rrtTime = 0;
		m_RRT->addMultipleNodes(5);
	}

	if (m_RRT->isPathFound()) {
		m_appInfo->updatePathStatus(true);
		m_appInfo->updatePathLength(m_RRT->getPathLenght());
	}
	m_appInfo->updatePathNodeCount(m_RRT->getNodeCount() - 1, maxNodeCount);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) || 
		(m_RRT->isPathFound() && m_RRT->getNodeCount() > maxNodeCount)) {
		m_applicationState = ApplicationState::Move;
		m_robot->goPath(m_RRT->getPath());
		m_appInfo->updatePathNodeCount(m_RRT->getNodeCount() - 1, maxNodeCount);
		m_RRT->reset();
		m_robot->setDrawParticleFilter(true);
	}
}

void Application::moveStateUpdate(float dt)
{
	m_robot->update(dt);
	m_appInfo->updateTrustLevel(m_robot->getEstimationTrust() * 100.0, 0.7 * 100.0);
	m_appInfo->updateCurrentPhase("RETURNING");
	m_appInfo->updatePathProgress(m_robot->getPathCompletion());

	if(!m_robot->isIdle())
		m_appInfo->updateCurrentPhase("RETURNING");
	else
		m_appInfo->updateCurrentPhase("FINISHED", true);
}

void Application::drawInterface()
{
	const int verticalLine_x = 1220;
	const int horizontalLine_y = 170;
	const int horizontalLine2_y = 340;

	m_InterfaceLine.setStartEndPos(sf::Vector2f(verticalLine_x, 0), sf::Vector2f(verticalLine_x, 900));
	m_window->draw(m_InterfaceLine);
	m_InterfaceLine.setStartEndPos(sf::Vector2f(verticalLine_x, horizontalLine_y), 
		sf::Vector2f(1600, horizontalLine_y));
	m_window->draw(m_InterfaceLine);
	m_InterfaceLine.setStartEndPos(sf::Vector2f(verticalLine_x, horizontalLine2_y),
		sf::Vector2f(1600, horizontalLine2_y));
	m_window->draw(m_InterfaceLine);
	m_greyBackground.setSize(sf::Vector2f(verticalLine_x, 900));
}

Application::Application(sf::RenderWindow* window)
{
	sf::Vector2f scenePos = sf::Vector2f(100, 100);
	m_scene = new Scene(scenePos);

	m_scene->setDrawWalls(false);

	m_whiteBackground.setFillColor(sf::Color::White);
	m_whiteBackground.setSize(m_scene->getSceneSize());
	m_whiteBackground.setPosition(scenePos);
	m_greyBackground.setFillColor(sf::Color(220, 220, 220));
	m_greyBackground.setPosition(0, 0);

	m_robot = new ControlRobot(m_scene);
	m_robotPosHandler = new RobotPosHandler(m_robot, m_scene);
	m_goalPosHandler = new GoalPosHandler(&m_goalMarkerShape);
	m_RRT = new RRT(m_scene);

	m_goalMarkerShape.setFillColor(sf::Color::Blue);
	m_goalMarkerShape.setRadius(16);
	m_goalMarkerShape.setOrigin(16, 16);
	m_goalMarkerShape.setPosition(700, 100);
	m_goalMarkerShape.setOutlineColor(sf::Color::Black);
	m_goalMarkerShape.setOutlineThickness(4);

	m_robot->setPos(sf::Vector2f(300, 600));
	m_window = window;

	m_applicationState = ApplicationState::Setup;

	m_startButton = new Button(1260, 30, "C:\\Users\\chedo\\OneDrive\\Pulpit\\PARTICLE_FILTER\\button1.png", 45);
	m_startButton->setCallback(std::bind(&Application::start, this));

	m_resetButton = new Button(1260, 95, "C:\\Users\\chedo\\OneDrive\\Pulpit\\PARTICLE_FILTER\\button2.png", 45);
	m_resetButton->setCallback(std::bind(&Application::reset, this));

	m_appInfo = new AppInfo(sf::Vector2f(1270, 180));

	const sf::Color interfaceColor = sf::Color(180, 180, 180);
	m_InterfaceLine.setFillColor(interfaceColor);
	m_InterfaceLine.setThicness(4);
	reset();
}

void Application::update(float dt)
{
	m_startButton->update((sf::Vector2f)sf::Mouse::getPosition(*m_window));
	m_resetButton->update((sf::Vector2f)sf::Mouse::getPosition(*m_window));

	switch (m_applicationState)
	{
		case ApplicationState::Setup:
			setupStateUpdate(dt);
			break;
		case ApplicationState::Estimation:
			estimationStateUpdate(dt);
			break;
		case ApplicationState::PathFinding:
			pathFindingStateUpdate(dt);
			break;
		case ApplicationState::Move:
			moveStateUpdate(dt);
			break;
	}
}

void Application::draw()
{
	m_window->draw(m_greyBackground);
	m_window->draw(m_whiteBackground);
	
	m_RRT->draw(*m_window);

	if (m_applicationState != ApplicationState::PathFinding) {
		m_robot->drawScanLines(*m_window);
		m_scene->draw(*m_window);
		m_robot->draw(*m_window);
	}
	else {
		m_scene->draw(*m_window);
		m_robot->draw(*m_window);
	}

	if (m_applicationState == ApplicationState::Setup) {
		m_robotPosHandler->draw(*m_window);
		m_goalPosHandler->draw(*m_window);
	}

	m_window->draw(m_goalMarkerShape);

	m_startButton->draw(*m_window);
	m_resetButton->draw(*m_window);
	m_appInfo->draw(*m_window);

	drawInterface();
}

void Application::start()
{
	if (m_applicationState == ApplicationState::Setup) {
		m_applicationState = ApplicationState::Estimation;
		m_appInfo->updateCurrentPhase("LOCALIZATION");
		m_robot->setDrawParticleFilter(true);
	}
}

void Application::reset()
{
	m_applicationState = ApplicationState::Setup;
	m_robot->reset();
	m_RRT->reset();
	m_robot->setDrawParticleFilter(false);

	m_robot->setPos(sf::Vector2f(100,100));
	m_goalMarkerShape.setPosition(m_scene->worldToScreen(sf::Vector2f(350,100)));
	
	m_appInfo->resetAll();
}
