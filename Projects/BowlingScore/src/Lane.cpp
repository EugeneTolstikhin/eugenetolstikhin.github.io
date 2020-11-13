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
        view->Draw(ViewElement::LANE);
    }

    m_Players.reserve(players.size());
    for (size_t i = 0; i < players.size(); ++i)
    {
        m_Players.emplace(m_Players.begin() + i, new Player(players.at(i), m_Views));
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
