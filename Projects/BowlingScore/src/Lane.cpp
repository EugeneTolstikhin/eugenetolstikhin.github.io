#include "Lane.h"
#include "Player.h"
#include "ViewFactory.h"

Lane::Lane() :
    m_factory(new ViewFactory),
    m_Views(1)
{
    m_Views.emplace(m_Views.begin(), m_factory->CreateView(m_type));
}

Lane::~Lane()
{
    //
}

void Lane::Init(const std::vector<std::string>& players)
{
    m_Players.reserve(players.size());
    for (size_t i = 0; i < players.size(); ++i)
    {
        m_Players.emplace(m_Players.begin() + i, new Player(players.at(i)));
    }

    for (auto& view : m_Views)
    {
        view->InitScoreTable(players);
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
