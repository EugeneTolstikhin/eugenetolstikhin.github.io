#include "Frame.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include <stdexcept>
#include <numeric>

Frame::Frame(bool isLastFrame, IView* view) :
    m_isLastFrame(isLastFrame)
    ,m_view(view)
    ,m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(m_typeLogger))
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);

    if (m_view != nullptr)
    {
        m_view->Draw(ViewElement::FRAME, &m_isLastFrame);
    }
    else
    {
        m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}

Frame::~Frame()
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
}

Flag& Frame::SetTrialPoints(const unsigned short points)
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    unsigned short p = points; //TMP
    switch (m_CurrentTrial)
    {
        case Trial::FIRST:
        {
            if (points > MAX_POINTS)
            {
                m_log->LogMe(__FILE__, __LINE__, std::string("Amount of points = ") + std::to_string(points));
                throw std::runtime_error("Amount of points is more then allowed for the 1st trial");
            }

            break;
        }
        case Trial::SECOND:
        {
            bool specialCase = m_isLastFrame && MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));

            if (!specialCase)
            {
                if (points > MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    p = MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));
                }
            }

            break;
        }
        case Trial::THIRD:
        {
            bool specialCase = m_isLastFrame && MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND));
            
            if (!m_isLastFrame)
            {
                throw std::runtime_error("Only the last frame contains the 3rd trial");
            }

            if (specialCase)
            {
                if (points > MAX_POINTS)
                {
                    throw std::runtime_error("Amount of points is more then allowed for the 2nd trial");
                }
            }
            else if (points > MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))
            {
                p = MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));
            }

            break;
        }
        default:
            break;
    }
    m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) = p;//points;

    incTrial(p);

    return m_Flag;
}

bool Frame::isAllowedThrow() const noexcept 
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    return m_AllowThrow;
}

unsigned short Frame::GetTotalFramePoints() const noexcept 
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    auto sum = std::accumulate(m_TrialPoints.begin(), m_TrialPoints.end(), 0);
    return sum;
}

void Frame::incTrial(const unsigned short points)
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    bool specialCase = m_isLastFrame && MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial));
    switch (m_CurrentTrial)
    {
        case Trial::FIRST:
        {
            if (!specialCase)
            {
                if (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)))
                {
                    m_Flag = m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) == 0
                                ? Flag::ZERO
                                : Flag::NOTHING;
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
                    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
                    m_log->LogMe(__FILE__, __LINE__, std::string("points = ") + std::to_string(points));
                    throw std::runtime_error("Unimplemented functionality");
                }
            }
            else
            {
                m_Flag = Flag::STRIKE;
                m_CurrentTrial = Trial::SECOND;
                m_AllowThrow = true;
            }
            break;
        }
        case Trial::SECOND:
        {
            if (!specialCase)
            {
                if (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    m_Flag = m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) == 0
                            ? Flag::ZERO
                            : Flag::NOTHING;
                    m_CurrentTrial = Trial::FIRST;
                    m_AllowThrow = false;
                }
                else if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                {
                    m_Flag = (m_isLastFrame && m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) == MAX_POINTS)
                        ? Flag::ZERO // Very special case at 10th: "X - <smth>"
                        : Flag::SPARE;
                    m_CurrentTrial = m_isLastFrame ? Trial::THIRD : Trial::FIRST;
                    m_AllowThrow = m_isLastFrame;
                }
                else
                {
                    m_log->LogMe(__FILE__, __LINE__, std::string("1st points = ") + std::to_string(m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST))) + std::string("2nd points = ") + std::to_string(m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))); // Check the conditions when the program enter here
                    
                    m_Flag = m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) == 0
                            ? Flag::ZERO
                            : Flag::NOTHING;
                    m_CurrentTrial = m_isLastFrame ? Trial::THIRD : Trial::FIRST;
                    m_AllowThrow = m_isLastFrame;
                }
            }
            else
            {
                m_Flag = m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) == MAX_POINTS
                        ? Flag::STRIKE
                        : Flag::SPARE;
                m_CurrentTrial = Trial::THIRD;
                m_AllowThrow = true;
            }
            break;
        }
        case Trial::THIRD:
        {
            if ((MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)))
            ||
                (m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)) + m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) == MAX_POINTS))
            {
                if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)))
                {
                    m_Flag = Flag::STRIKE;
                }
                else if (m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) == 0)
                {
                    m_Flag = Flag::ZERO;
                }
                else
                {
                    m_Flag = Flag::NOTHING;
                }
            }
            else if ((MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)))
                 &&
                     (MAX_POINTS > m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND))))
            {
                if (MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::SECOND)) + m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)))
                {
                    m_Flag = Flag::SPARE;
                }
                else if (0 == m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)))
                {
                    m_Flag = Flag::ZERO;
                }
                else
                {
                    m_Flag = Flag::NOTHING;
                }
            }
            
            m_CurrentTrial = Trial::FIRST;
            m_AllowThrow = false; 

            break;
        }
        default:
            break;
    }

    if (m_view != nullptr)
    {
        m_view->UpdateFrameScore(points, m_Flag);
        m_view->SetNextFrameActive(false);
    }
    else
    {
        m_log->LogMe(__FILE__, __LINE__, "view is UNAVAILABLE");
    }
}

bool Frame::isLastFrame() const noexcept
{
    m_log->LogMe(__FILE__, __LINE__, __FUNCTION__);
    return m_isLastFrame;
}
