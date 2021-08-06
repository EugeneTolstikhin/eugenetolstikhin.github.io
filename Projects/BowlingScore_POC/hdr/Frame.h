#ifndef __FRAME_H__
#define __FRAME_H__

#include "IFrame.h"
#include "IView.h"
#include "ILoggerFactory.h"
#include <memory>

class Frame : public IFrame
{
public:

    Frame();
    Frame(bool, const std::vector<std::shared_ptr<IView>>&);
    virtual ~Frame();

    Frame(const Frame&) = delete;
    Frame(Frame&&) = delete;
    Frame& operator = (const Frame&) = delete;
    Frame&& operator = (Frame&&) = delete;

    virtual Flag& SetTrialPoints(const unsigned short) override;
    virtual bool isAllowedThrow() const noexcept override;
    virtual unsigned short GetTotalFramePoints() const noexcept override;
    virtual bool isLastFrame() const noexcept override;

private:
    void incTrial(const unsigned short points);

    Flag m_Flag = Flag::NOTHING;
    bool m_isLastFrame = false;
    bool m_AllowThrow = true;
    Trial m_CurrentTrial = Trial::FIRST;
    std::vector<unsigned short> m_TrialPoints = {0, 0, 0};
    std::vector<std::shared_ptr<IView>> m_Views;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif // __FRAME_H__
