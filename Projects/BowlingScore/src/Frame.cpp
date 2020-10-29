#include "Frame.h"
#include <stdexcept>

void Frame::SetFlag(const Flag& flag) noexcept
{
    m_Flag = flag;
}

void Frame::SetLastFrame(bool flag) noexcept
{
    m_isLastFrame = flag;
}

void Frame::SetCurrentTrialIndex(const Trial& trial) noexcept
{
    m_currentTrial = trial;
}

void Frame::SetTrialPoints(const unsigned short points) 
{
    switch (m_currentTrial)
    {
        case Trial::FIRST:
        {
            if (points > MAX_POINTS)
                throw std::runtime_error("Amount of points is more then allowed for the 1st trial");
        }
        case Trial::SECOND:
        {
            bool specialCase = m_isLastFrame && 10 == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));
            if (points > MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) && false == specialCase)
                throw std::runtime_error("Amount of points is more then allowed for the 2nd trial"); 
        }
        case Trial::THIRD:
        {
            if (m_isLastFrame)
                throw std::runtime_error("Amount of points is more then allowed for the 2nd trial");

            if (points > MAX_POINTS)
                throw std::runtime_error("Amount of points is more then allowed for the 2nd trial"); 
        }
        default:
            break;
    }
    m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial)) = points;

    incTrial();
}

Flag Frame::GetFlag() const noexcept
{
    return m_Flag;
}

bool Frame::isLastFrame() const noexcept
{
    return m_isLastFrame;
}

Trial Frame::GetCurrentTrialIndex() const noexcept
{
    return m_currentTrial;
}

unsigned short Frame::GetTrialPoints(const Trial& trial) const noexcept
{
    return m_TrialPoints.at(static_cast<unsigned short>(trial));
}

void Frame::incTrial()
{
    bool specialCase = m_isLastFrame && 10 == m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial));
    switch (m_currentTrial)
    {
        case Trial::FIRST:
        {
            if (!specialCase)
            {
                if (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial)))
                {
                    m_Flag = Flag::NOTHING;
                    m_currentTrial = Trial::SECOND;
                    allowThrow = true;
                }
                else if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial)))
                {
                    m_Flag = Flag::STRIKE;
                    allowThrow = false;
                }
                else
                {
                    //
                }
            }
            else
            {
                //
            }
            break;
        }
        case Trial::SECOND:
        {
            if (!specialCase)
            {
                if (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    m_Flag = Flag::NOTHING;
                    m_currentTrial = Trial::FIRST;
                    allowThrow = !m_isLastFrame;
                }
                else if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    m_Flag = Flag::SPARE;
                    m_currentTrial = m_isLastFrame ? Trial::THIRD : Trial::FIRST;
                    allowThrow = m_isLastFrame;
                }
                else
                {
                    //
                }
            }
            else
            {
                //
            }
        }
        case Trial::THIRD:
        {
            if ((MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))
            ||
                (m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) == MAX_POINTS)
            )
            {
                m_Flag = (10 == m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial))) ? Flag::STRIKE : Flag::NOTHING;
            }
            else if ((MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                 &&
                     (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))
            )
            {
                m_Flag = (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)) + m_TrialPoints.at(static_cast<unsigned short>(m_currentTrial))) ? Flag::SPARE : Flag::NOTHING;
            }
            
            m_currentTrial = Trial::FIRST;
            allowThrow = false; 
        }
        default:
            break;
    }
}
