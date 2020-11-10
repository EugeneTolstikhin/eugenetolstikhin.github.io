#include "Lane.h"
#include "Player.h"
#include "ViewFactory.h"
#include "LoggerFactory.h"

Lane::Lane() :
    m_factoryViews(new ViewFactory),
    m_factoryLogger(new LoggerFactory),
    m_logger(m_factoryLogger->CreateLogger(m_typeLogger))
{
    m_Views.emplace_back(m_factoryViews->CreateView(m_typeView));
}

Lane::~Lane()
{
    //
}

void Lane::Init(const std::vector<std::string>& players)
{
    for (auto& view : m_Views)
    {
        view->InitScoreTableFrame(players);
    }

    //m_logger->LogMe(__FILE__, __LINE__, std::string("Amount of views = ") + std::to_string(m_Views.size()));

    m_Players.reserve(players.size());
    for (size_t i = 0; i < players.size(); ++i)
    {
        m_Players.emplace(m_Players.begin() + i, new Player(players.at(i), m_Views));
    }

    //m_logger->LogMe(__FILE__, __LINE__, std::string("Amount of players = ") + std::to_string(m_Players.size()));

    for (auto& view : m_Views)
    {
        view->InitFlush();
    }
}

void Lane::Play(std::function<void()> gameover)
{
    size_t counterGameovers = 0;
    for (auto& player: m_Players)
    {
        player->Play(m_Views, [&counterGameovers]{
            ++counterGameovers;
        });
    }

    if (counterGameovers == m_Players.size())
    {
        gameover();
    }
}

void Lane::Finish()
{
    m_Players.clear();
    for (auto& view : m_Views)
    {
        view->CleanScore();
    }
}
