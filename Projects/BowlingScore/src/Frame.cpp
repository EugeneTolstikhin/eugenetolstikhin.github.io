#include "Frame.h"
#include <stdexcept>

Frame::Frame()
{
    //
}

Frame::Frame(bool isLastFrame, const std::vector<std::shared_ptr<IView>>& views) :
    m_isLastFrame(isLastFrame)
    ,m_Views(views)
{
    for (auto& view : m_Views)
    {
        view->Draw(ViewElement::FRAME, &m_isLastFrame);
    }
}

Frame::~Frame()
{
    //
}

void Frame::SetTrialPoints(const unsigned short points) 
{
    switch (m_CurrentTrial)
    {
        case Trial::FIRST:
        {
            if (points > MAX_POINTS)
                throw std::runtime_error("Amount of points is more then allowed for the 1st trial");
        }
        case Trial::SECOND:
        {
            bool specialCase = m_isLastFrame && MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));
            if (points > MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) && false == specialCase)
                throw std::runtime_error("Amount of points is more then allowed for the 2nd trial"); 
        }
        case Trial::THIRD:
        {
            if (!m_isLastFrame)
                throw std::runtime_error("Only the last frame contains the 3rd trial");

            if (points > MAX_POINTS)
                throw std::runtime_error("Amount of points is more then allowed for the 2nd trial"); 
        }
        default:
            break;
    }
    m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) = points;

    incTrial();
}

bool Frame::isAllowedThrow() const noexcept 
{
    return m_AllowThrow;
}

void Frame::incTrial()
{
    bool specialCase = m_isLastFrame && MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial));
    switch (m_CurrentTrial)
    {
        case Trial::FIRST:
        {
            if (!specialCase)
            {
                if (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)))
                {
                    m_Flag = Flag::NOTHING;
                    m_CurrentTrial = Trial::SECOND;
                    m_AllowThrow = true;
                }
                else if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)))
                {
                    m_Flag = Flag::STRIKE;
                    m_CurrentTrial = m_isLastFrame ? Trial::SECOND : Trial::FIRST;
                    m_AllowThrow = m_isLastFrame;
                }
                else
                {
                    throw std::runtime_error("Unimplemented functionality");
                }
            }
            else
            {
                throw std::runtime_error("Unimplemented functionality");
            }
            break;
        }
        case Trial::SECOND:
        {
            if (!specialCase)
            {
                if (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    m_Flag = Flag::NOTHING;
                    m_CurrentTrial = Trial::FIRST;
                    m_AllowThrow = false;
                }
                else if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    m_Flag = Flag::SPARE;
                    m_CurrentTrial = m_isLastFrame ? Trial::THIRD : Trial::FIRST;
                    m_AllowThrow = m_isLastFrame;
                }
                else
                {
                    throw std::runtime_error("Unimplemented functionality");
                }
            }
            else
            {
                throw std::runtime_error("Unimplemented functionality");
            }
        }
        case Trial::THIRD:
        {
            if ((MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))
            ||
                (m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) == MAX_POINTS)
            )
            {
                m_Flag = (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial))) ? Flag::STRIKE : Flag::NOTHING;
            }
            else if ((MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                 &&
                     (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))
            )
            {
                m_Flag = (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)) + m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial))) ? Flag::SPARE : Flag::NOTHING;
            }
            
            m_CurrentTrial = Trial::FIRST;
            m_AllowThrow = false; 
        }
        default:
            break;
    }
}
