#pragma once

#include "ControlRobot.h"
#include "Scene.h"
#include "RobotPosHandler.h"
#include "GoalPosHandler.h"
#include "RRT.h"
#include "Button.h"
#include "AppInfo.h"

enum class ApplicationState {Setup, Estimation, PathFinding, Move};

class Application
{
	ApplicationState m_applicationState;
	void setupStateUpdate(float dt);
	void estimationStateUpdate(float dt);
	void pathFindingStateUpdate(float dt);
	void moveStateUpdate(float dt);

	Scene* m_scene;
	ControlRobot* m_robot;
	RRT* m_RRT;
	sf::RenderWindow* m_window;
	RobotPosHandler* m_robotPosHandler;

	GoalPosHandler* m_goalPosHandler;
	sf::CircleShape m_goalMarkerShape;

	float m_rrtTime;

	Button* m_startButton;
	Button* m_resetButton;
	AppInfo* m_appInfo;

	sf::RectangleShape m_greyBackground;
	sf::RectangleShape m_whiteBackground;
	LineShape m_InterfaceLine;
	void drawInterface();

public:
	Application(sf::RenderWindow *window);
	void update(float dt);
	void draw();

	void start();
	void reset();
};

