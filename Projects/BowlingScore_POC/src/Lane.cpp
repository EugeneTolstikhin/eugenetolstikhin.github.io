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
{
    m_Views.emplace_back(m_factoryViews->CreateView(m_typeView));
}

Lane::~Lane()
{
    //
}

void Lane::Init(const std::vector<std::string>& players)
{
    auto getPoints = [this]()->unsigned short {
        m_listener->Connect();
        unsigned short res = m_listener->Receive();
        m_listener->Shutdown();
        return res;
    };
    if (players.size() > 0)
    {
        m_Players.reserve(players.size());
        for (size_t i = 0; i < players.size(); ++i)
        {
            m_Players.emplace(m_Players.begin() + i, new Player(players.at(i), m_Views, getPoints));
        }
    }
    else
    {
        m_Players.emplace_back(new Player(m_Views, getPoints));
    }

    for (auto& view : m_Views)
    {
        view->Draw(ViewElement::FLUSH);
    }
}

void Lane::Play(std::function<void()> gameover)
{
    size_t counterGameovers = 0;
    while (counterGameovers < m_Players.size())
    {
        for (auto& player: m_Players)
        {
            player->Play(m_Views, [&counterGameovers]{
                ++counterGameovers;
            });
        }
    }

    gameover();
}

void Lane::Finish()
{
    m_Players.clear();
    for (auto& view : m_Views)
    {
        view->CleanScore();
    }
}
