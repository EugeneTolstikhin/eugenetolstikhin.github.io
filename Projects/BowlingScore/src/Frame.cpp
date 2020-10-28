#include "Frame.h"

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

void Frame::SetTrialPoints(const Trial& trial, const unsigned short points) 
{
    m_TrialPoints.at(static_cast<unsigned short>(trial)) = points;
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

unsigned short Frame::GetTrialPoints(const Trial& trial) const
{
    return m_TrialPoints.at(static_cast<unsigned short>(trial));
}
