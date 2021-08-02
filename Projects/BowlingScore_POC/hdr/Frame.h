#ifndef __FRAME_H__
#define __FRAME_H__

#include "IFrame.h"
#include "IView.h"
#include "ILoggerFactory.h"
#include <memory>
#include <vector>

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

    virtual void SetTrialPoints(const unsigned short) override;
    virtual bool isAllowedThrow() const noexcept override;
    virtual unsigned short GetTotalFramePoints() const noexcept override;

private:
    void incTrial();

    Flag m_Flag = Flag::NOTHING;
    bool m_isLastFrame = false;
    Trial m_CurrentTrial = Trial::FIRST;
    std::vector<unsigned short> m_TrialPoints = {0, 0, 0};
    bool m_AllowThrow = true;
    std::vector<std::shared_ptr<IView>> m_Views;

    std::unique_ptr<ILoggerFactory> m_loggerFactory;
    std::unique_ptr<ILogger> m_log;
};

#endif // __FRAME_H__
