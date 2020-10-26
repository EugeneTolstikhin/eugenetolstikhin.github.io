#ifndef __FRAME_H__
#define __FRAME_H__

#include "IFrame.h"

class Frame : public IFrame
{
public:
    Frame();
    virtual ~Frame();

    virtual void SetFlag(const Flag&);
    virtual void Set10thFrame(bool);
    virtual void SetCurrentTrialNumber(const Trial&);

    virtual Flag GetFlag() const;
    virtual bool is10thFrame() const;
    virtual Trial GetCurrentTrialNumber() const;
private:
    Flag m_Flag = Flag::NOTHING;
    bool m_10thFrame = false;
    Trial m_currentTrialNumber = Trial::ZERO;
};

#endif // __FRAME_H__
