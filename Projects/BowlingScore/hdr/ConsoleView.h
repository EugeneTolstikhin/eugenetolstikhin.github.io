#ifndef __CONSOLE_VIEW_H__
#define __CONSOLE_VIEW_H__

#include "IView.h"
#include "ILoggerFactory.h"
#include <string>
#include <ncurses.h>
#include <memory>
#include <vector>

typedef std::pair<WINDOW*, Flag> FRAME;
typedef std::vector<FRAME> FRAMES;
typedef std::vector<FRAMES> GAME;
typedef std::pair<std::string, WINDOW*> PLAYER;

class ConsoleView : public IView
{
public:
    ConsoleView();
    virtual ~ConsoleView();

    ConsoleView(const ConsoleView&) = delete;
    ConsoleView(ConsoleView&&) = delete;
    ConsoleView& operator = (const ConsoleView&) = delete;
    ConsoleView&& operator = (ConsoleView&&) = delete;

    virtual void Draw(const ViewElement&, void*) override;
    virtual void UpdateFrameScore(const unsigned short, const Flag&) override;
    virtual void UpdateScore(const unsigned short, const short, const bool) override;
    virtual void CleanScore() override;
    virtual void SetNextFrameActive(const bool) override;

private:
    void DrawPlayerScore(const std::string&);
    void DrawFrameScore(const bool);
    void SetNextPlayerActive();
    void PrintPlayerName(WINDOW*, const std::string&);
    void UpdateScoreFrame(const unsigned short, const short = 0);

    bool m_canBeColored = false;

    short m_ActivePlayerIdx = 0;
    short m_ActiveFramesIdx = 0;
    short m_ActiveFrameIdx = 0;

    size_t m_RowWidth = 0;
    size_t m_nameWidth = 0;
    size_t m_columnAmount = 0;
    size_t m_normalFrameCellsAmount = 0;
    size_t m_normalFramesAmount = 0;
    size_t m_lastFrameCellsAmount = 0;
    size_t m_cellWidth = 0;

    std::string m_PlayerGame;
    std::string m_ScoreTable;

    GAME m_Game;

    LoggerType m_typeLogger = LoggerType::TO_FILE;

    std::vector<PLAYER> m_wPlayers;
    std::vector<GAME> m_wGames;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;

    FRAME* m_ActiveFrame = nullptr;

    const unsigned int SLEEP_TIME = 1000000;
};

#endif //__CONSOLE_VIEW_H__
