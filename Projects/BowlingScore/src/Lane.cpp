#include "Lane.h"
#include "Player.h"

Lane::Lane() : m_Players(1)
{
    m_Players.emplace(m_Players.begin(), new Player);
}

Lane::Lane(const std::vector<std::string>& players) : m_Players(players.size())
{
    for (size_t i = 0; i < players.size(); ++i)
    {
        m_Players.emplace(m_Players.begin() + i, new Player(players.at(i)));
    }
}

Lane::~Lane()
{
    //
}

void Lane::Play()
{
    for (auto& player: m_Players)
    {
        player->Play();
    }
}