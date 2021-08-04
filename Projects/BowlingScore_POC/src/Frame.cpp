#include "Frame.h"
#include "LoggerFactory.h"
#include "FileLogger.h"
#include <stdexcept>
#include <numeric>

Frame::Frame()
{
    //
}

Frame::Frame(bool isLastFrame, const std::vector<std::shared_ptr<IView>>& views) :
    m_isLastFrame(isLastFrame)
    ,m_Views(views)
    ,m_loggerFactory(new LoggerFactory)
    ,m_log(m_loggerFactory->CreateLogger(LoggerType::TO_FILE))
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
    //m_log->LogMe(__FILE__, __LINE__, std::string("Amount of points are ") + std::to_string(points));
    unsigned short p = points; //TMP
    switch (m_CurrentTrial)
    {
        case Trial::FIRST:
        {
            if (points > MAX_POINTS)
                throw std::runtime_error("Amount of points is more then allowed for the 1st trial");

            break;
        }
        case Trial::SECOND:
        {
            bool specialCase = m_isLastFrame && MAX_POINTS == m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));
            if (points > MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST)) && false == specialCase)
            {
                //throw std::runtime_error("Amount of points is more then allowed for the 2nd trial");

                p = MAX_POINTS - m_TrialPoints.at(static_cast<unsigned short>(Trial::FIRST));
            }

            break;
        }
        case Trial::THIRD:
        {
            if (!m_isLastFrame)
                throw std::runtime_error("Only the last frame contains the 3rd trial");

            if (points > MAX_POINTS)
                throw std::runtime_error("Amount of points is more then allowed for the 2nd trial");

            break;
        }
        default:
            break;
    }
    m_TrialPoints.at(static_cast<unsigned short>(m_CurrentTrial)) = p;//points;

    incTrial(p);
}

bool Frame::isAllowedThrow() const noexcept 
{
    return m_AllowThrow;
}

unsigned short Frame::GetTotalFramePoints() const noexcept 
{
    auto sum = std::accumulate(m_TrialPoints.begin(), m_TrialPoints.end(), 0);
    return sum;
}

void Frame::incTrial(const unsigned short points)
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
            break;
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

            break;
        }
        default:
            break;
    }

    //m_log->LogMe(__FILE__, __LINE__, std::string("m_AllowThrow = ") + std::to_string(m_AllowThrow));

    for (auto& view : m_Views)
    {
        view->UpdateFrameScore(points, m_Flag);
        view->SetNextFrameActive(false);
    }
}
