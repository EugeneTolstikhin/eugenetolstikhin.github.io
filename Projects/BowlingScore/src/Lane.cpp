#include "Lane.h"
#include "Player.h"
#include "ViewFactory.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include "IPointsListener.h"
#include "PointsListenerFactory.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <string>
#include <string_view>

namespace
{
constexpr std::string_view DEFAULT_VIEW = "cls";
constexpr std::string_view TEXT_VIEW = "text";
constexpr std::string_view DEFAULT_LISTENER = "simulation";
constexpr std::string_view LOCAL_LISTENER = "local";
constexpr std::string_view NETWORK_LISTENER = "network";

std::string normaliseValue(const char* value)
{
    std::string normalised = value == nullptr ? "" : value;
    std::ranges::transform(normalised, normalised.begin(), [](const unsigned char ch)
    {
        return static_cast<char>(std::tolower(ch));
    });

    return normalised;
}

ViewType resolveViewType()
{
    const auto envView = normaliseValue(std::getenv("BOWLING_VIEW"));
    const std::string_view view = envView.empty() ? DEFAULT_VIEW : envView;

    if (view == TEXT_VIEW)
    {
        return ViewType::TEXT;
    }

    return ViewType::CLS;
}

ListenerType resolveListenerType()
{
    const auto envListener = normaliseValue(std::getenv("BOWLING_LISTENER"));
    const std::string_view listener = envListener.empty() ? DEFAULT_LISTENER : envListener;

    if (listener == LOCAL_LISTENER)
    {
        return ListenerType::LOCAL;
    }

    if (listener == NETWORK_LISTENER)
    {
        return ListenerType::NETWORK;
    }

    return ListenerType::SIMULATION;
}
}

Lane::Lane() :
    m_typeView(resolveViewType())
    ,m_typeLogger(LoggerType::TO_FILE)
    ,m_listenerType(resolveListenerType())
    ,m_factoryViews(std::make_unique<ViewFactory>())
    ,m_factoryLogger(std::make_unique<LoggerFactory>())
    ,m_log(m_factoryLogger->CreateLogger(m_typeLogger))
    ,m_pointsListenerFactory(std::make_unique<PointsListenerFactory>())
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
    const auto getPoints = [this]() -> unsigned short
    {
		return m_listener->Receive();
    };

    if (!players.empty())
    {
        m_Players.reserve(players.size());
        for (const auto& playerName : players)
        {
            m_Players.emplace_back(std::make_unique<Player>(playerName, m_view.get(), getPoints));
        }
    }
    else
    {
        m_Players.emplace_back(std::make_unique<Player>(m_view.get(), getPoints));
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

