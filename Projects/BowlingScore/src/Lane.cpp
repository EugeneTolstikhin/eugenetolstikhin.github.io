#include "Lane.h"
#include "Player.h"
#include "ViewFactory.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include "IPointsListener.h"
#include "PointsListenerFactory.h"

Lane::Lane() :
    m_factoryViews(new ViewFactory)
    ,m_factoryLogger(new LoggerFactory)
    ,m_log(m_factoryLogger->CreateLogger(m_typeLogger))
    ,m_pointsListenerFactory(new PointsListenerFactory)
    ,m_listener(m_pointsListenerFactory->CreatePointsListener(m_listenerType))
    ,m_view(m_factoryViews->CreateView(m_typeView))
{
	//
}

Lane::~Lane()
{
    //
}

void Lane::Init(const std::vector<std::string>& players)
{
    auto getPoints = [this]()->unsigned short {
		return m_listener->Receive();
    };
    if (players.size() > 0)
    {
        m_Players.reserve(players.size());
        for (size_t i = 0; i < players.size(); ++i)
        {
            m_Players.emplace(m_Players.begin() + i, new Player(players.at(i), m_view.get(), getPoints));
        }
    }
    else
    {
        m_Players.emplace_back(new Player(m_view.get(), getPoints));
    }

    if (m_view.get() != nullptr)
    {
        m_view->Draw(ViewElement::FLUSH);
    }
    else
    {
        m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}

void Lane::Play(std::function<void()> gameover)
{
    size_t counterGameovers = 0;
    while (counterGameovers < m_Players.size())
    {
        for (auto& player: m_Players)
        {
            player->Play([&counterGameovers]{
                ++counterGameovers;
            });
        }
    }

    gameover();
}

void Lane::Finish()
{
    m_Players.clear();

    if (m_view.get() != nullptr)
    {
        m_view->CleanScore();
    }
    else
    {
        m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}
