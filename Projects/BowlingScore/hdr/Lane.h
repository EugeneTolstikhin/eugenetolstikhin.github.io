#ifndef __LANE_H__
#define __LANE_H__

#include "ILane.h"
#include "IPlayer.h"
#include "IViewFactory.h"
#include "IView.h"
#include "ILoggerFactory.h"
#include "ILogger.h"

class Lane : public ILane
{
public:
    Lane();
    virtual ~Lane();

    Lane(const Lane&) = delete;
    Lane(Lane&&) = delete;
    Lane& operator = (const Lane&) = delete;
    Lane&& operator = (Lane&&) = delete;

    virtual void Init(const std::vector<std::string>&) override;
    virtual void Play(std::function<void()> gameover) override;
    virtual void Finish() override;
    
private:
    ViewType m_typeView = ViewType::CLS;
    LoggerType m_typeLogger = LoggerType::CLS;
    std::vector<std::unique_ptr<IPlayer>> m_Players;
    std::unique_ptr<IViewFactory> m_factoryViews;
    std::vector<std::shared_ptr<IView>> m_Views;
    std::unique_ptr<ILoggerFactory> m_factoryLogger;
    std::shared_ptr<ILogger> m_logger;
};

#endif // __LANE_H__
