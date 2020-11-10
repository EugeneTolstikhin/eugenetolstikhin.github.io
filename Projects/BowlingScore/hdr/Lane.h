#ifndef __LANE_H__
#define __LANE_H__

#include "ILane.h"
#include "IPlayer.h"
#include "IViewFactory.h"
#include "IView.h"

class Lane : public ILane
{
public:
    Lane();
    virtual ~Lane();

    virtual void Init(const std::vector<std::string>&);
    virtual void Play(std::function<void()> gameover);
    virtual void Finish();
    
private:
    ViewType m_type = ViewType::CLS;
    std::vector<std::unique_ptr<IPlayer>> m_Players;
    std::unique_ptr<IViewFactory> m_factory;
    std::vector<std::shared_ptr<IView>> m_Views;
};

#endif // __LANE_H__
