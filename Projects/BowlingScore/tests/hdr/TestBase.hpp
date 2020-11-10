#ifndef __TESTBASE_H__
#define __TESTBASE_H__

#include <gtest/gtest.h>
#include "FrameTest.hpp"
#include "GameTest.hpp"
#include "LaneTest.hpp"
#include "PlayerTest.hpp"
#include "ConsoleViewTest.hpp"
#include "PointsListenerSimulationTest.hpp"
#include "PointsListenerLocalTest.hpp"
#include "PointsListenerFactoryTest.hpp"

class TestBase : public testing::Test
{
public:
    TestBase();
    virtual ~TestBase() override;

    virtual void SetUp() override;
    virtual void TearDown() override;

    static void SetUpTestCase();
	static void TearDownTestCase();

public:
	static int mArgc;
	static char** mArgs;

	static TestFrame* pFrame;
	static TestGame* pGame;
	static TestLane* pLane;
	static TestPlayer* pPlayer;
	static TestConsoleView* pConsoleView;
	static TestPointsListenerSimulation* pSimulationListener;
	static TestPointsListenerLocal* pLocalListener;
	static TestPointsListenerFactory* pListenerFactory;
};

int TestBase::mArgc = 0;
char** TestBase::mArgs = NULL;

TestFrame* TestBase::pFrame = nullptr;
TestGame* TestBase::pGame = nullptr;
TestLane* TestBase::pLane = nullptr;
TestPlayer* TestBase::pPlayer = nullptr;
TestConsoleView* TestBase::pConsoleView = nullptr;
TestPointsListenerSimulation* TestBase::pSimulationListener = nullptr;
TestPointsListenerLocal* TestBase::pLocalListener = nullptr;
TestPointsListenerFactory* TestBase::pListenerFactory = nullptr;

void TestBase::SetUpTestCase()
{
	std::cout << "Set up test cases." << std::endl;

	if ( pFrame != nullptr )
	{
		delete pFrame;
	}

	if ( pGame != nullptr )
	{
		delete pGame;
	}

	if ( pLane != nullptr )
	{
		delete pLane;
	}

	if ( pPlayer != nullptr )
	{
		delete pPlayer;
	}

	if ( pConsoleView != nullptr )
	{
		delete pConsoleView;
	}

	if ( pSimulationListener != nullptr )
	{
		delete pSimulationListener;
	}

	if ( pLocalListener != nullptr )
	{
		delete pLocalListener;
	}

	if ( pListenerFactory != nullptr )
	{
		delete pListenerFactory;
	}

	pFrame = new TestFrame;
	pGame = new TestGame;
	pLane = new TestLane;
	pPlayer = new TestPlayer;
	pConsoleView = new TestConsoleView;
	pSimulationListener = new TestPointsListenerSimulation;
	pLocalListener = new TestPointsListenerLocal;
	pListenerFactory = new TestPointsListenerFactory;
}

void TestBase::TearDownTestCase()
{
	std::cout << "Tear down test cases." << std::endl;

	delete pFrame;
	delete pGame;
	delete pLane;
	delete pPlayer;
	delete pConsoleView;
	delete pSimulationListener;
	delete pLocalListener;
	delete pListenerFactory;

    pFrame = nullptr;
	pGame = nullptr;
	pLane = nullptr;
	pPlayer = nullptr;
	pConsoleView = nullptr;
	pSimulationListener = nullptr;
	pLocalListener = nullptr;
	pListenerFactory = nullptr;
}
#endif // __TESTBASE_H__
