#ifndef __VIEW_FACTORY_H__
#define __VIEW_FACTORY_H__

#include "IViewFactory.h"

class ViewFactory : public IViewFactory
{
public:
    ViewFactory();
    virtual ~ViewFactory();

    ViewFactory(const ViewFactory&) = delete;
    ViewFactory(ViewFactory&&) = delete;
    ViewFactory& operator = (const ViewFactory&) = delete;
    ViewFactory&& operator = (ViewFactory&&) = delete;

    virtual IView* CreateView(const ViewType&) override;
};

#endif //__VIEW_FACTORY_H__
