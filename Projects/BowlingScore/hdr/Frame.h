#ifndef __FRAME_H__
#define __FRAME_H__

#include "IFrame.h"
#include <vector>

class Frame : public IFrame
{
public:

    Frame() = default;
    virtual ~Frame() = default;

    Frame(const Frame&) = delete;
    Frame(Frame&&) = delete;
    Frame& operator = (const Frame&) = delete;
    Frame&& operator = (Frame&&) = delete;

    virtual void SetFlag(const Flag&) noexcept override;
    virtual void SetLastFrame(const bool) noexcept override;
    virtual void SetCurrentTrialIndex(const Trial&) noexcept override;
    virtual void SetTrialPoints(const unsigned short) override;

    virtual Flag GetFlag() const noexcept override;
    virtual bool isLastFrame() const noexcept override;
    virtual Trial GetCurrentTrialIndex() const noexcept override;
    virtual unsigned short GetTrialPoints(const Trial&) const noexcept override;

private:
    void incTrial();

    Flag m_Flag = Flag::NOTHING;
    bool m_isLastFrame = false;
    Trial m_currentTrial = Trial::ZERO;
    std::vector<unsigned short> m_TrialPoints = {0, 0, 0};
    bool allowThrow = true;
};

#endif // __FRAME_H__
