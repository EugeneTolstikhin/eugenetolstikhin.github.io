#ifndef __VIEW_FACTORY_H__
#define __VIEW_FACTORY_H__

#include "IViewFactory.h"

class ViewFactory : public IViewFactory
{
public:
    ViewFactory();
    virtual ~ViewFactory();

    virtual IView* CreateView(const ViewType&);
};

#endif //__VIEW_FACTORY_H__
