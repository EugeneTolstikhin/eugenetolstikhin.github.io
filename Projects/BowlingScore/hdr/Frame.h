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

    virtual void SetFlag(const Flag&) override;
    virtual void Set10thFrame(const bool) override;
    virtual void SetCurrentTrialNumber(const Trial&) override;
    virtual void SetTrialPoints(const Trial&, const unsigned short) override;

    virtual Flag GetFlag() const override;
    virtual bool is10thFrame() const override;
    virtual Trial GetCurrentTrialNumber() const override;
    virtual unsigned short GetTrialPoints(const Trial&) const override;

private:

    Flag m_Flag = Flag::NOTHING;
    bool m_10thFrame = false;
    Trial m_currentTrialNumber = Trial::ZERO;
    std::vector<unsigned short> m_TrialPoints = {0, 0, 0};
};

#endif // __FRAME_H__
