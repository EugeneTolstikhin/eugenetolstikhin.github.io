#include <gtest/gtest.h>
#include "FrameTest.hpp"

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
};

int TestBase::mArgc = 0;
char** TestBase::mArgs = NULL;

TestFrame* TestBase::pFrame = NULL;

void TestBase::SetUpTestCase()
{
	std::cout << "Set up test cases." << std::endl;

	if ( pFrame != NULL )
	{
		delete pFrame;
	}

	pFrame = new TestFrame;
}

void TestBase::TearDownTestCase()
{
	std::cout << "Tear down test cases." << std::endl;

	delete pFrame;
    pFrame = NULL;
}