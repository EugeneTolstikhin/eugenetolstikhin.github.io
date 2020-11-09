#ifndef __FRAME_H__
#define __FRAME_H__

#include "IFrame.h"
#include <vector>

class Frame : public IFrame
{
public:

    Frame();
    virtual ~Frame()

    Frame(const Frame&) = delete;
    Frame(Frame&&) = delete;
    Frame& operator = (const Frame&) = delete;
    Frame&& operator = (Frame&&) = delete;

    virtual void SetTrialPoints(const unsigned short) override;
    virtual bool isAllowedThrow() const noexcept override;

private:
    void incTrial();

    Flag m_Flag = Flag::NOTHING;
    bool m_isLastFrame = false;
    Trial m_CurrentTrial = Trial::FIRST;
    std::vector<unsigned short> m_TrialPoints = {0, 0, 0};
    bool m_AllowThrow = true;
};

#endif // __FRAME_H__
