#include "Frame.h"

Frame::Frame()
{
    //
}

Frame::~Frame()
{
    //
}

void Frame::SetFlag(const Flag& flag) 
{
    m_Flag = flag;
}

void Frame::Set10thFrame(bool flag) 
{
    m_10thFrame = flag;
}

void Frame::SetCurrentTrialNumber(const Trial& trial) 
{
    m_currentTrialNumber = trial;
}

Flag Frame::GetFlag() const
{
    return m_Flag;
}

bool Frame::is10thFrame() const
{
    return m_10thFrame;
}

Trial Frame::GetCurrentTrialNumber() const
{
    return m_currentTrialNumber;
}
