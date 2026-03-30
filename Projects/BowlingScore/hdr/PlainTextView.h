#ifndef __PLAIN_TEXT_VIEW_H__
#define __PLAIN_TEXT_VIEW_H__

#include "IView.h"

#include <string>
#include <vector>

class PlainTextView : public IView
{
public:
    PlainTextView() = default;
    virtual ~PlainTextView() = default;

    PlainTextView(const PlainTextView&) = delete;
    PlainTextView(PlainTextView&&) = delete;
    PlainTextView& operator = (const PlainTextView&) = delete;
    PlainTextView&& operator = (PlainTextView&&) = delete;

    virtual void Draw(const ViewElement&, void* params = nullptr) override;
    virtual void UpdateFrameScore(const unsigned short, const Flag&) override;
    virtual void UpdateScore(const unsigned short, const short, const bool gameover = false) override;
    virtual void CleanScore() override;
    virtual void SetNextFrameActive(const bool) override;

private:
    const std::string& getActivePlayerName() const;
    static const char* flagToString(const Flag&);

    short m_activePlayerIdx = 0;
    short m_activeFrameIdx = 0;
    short m_activeThrowIdx = 0;

    std::vector<std::string> m_players;
};

#endif // __PLAIN_TEXT_VIEW_H__
