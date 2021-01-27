#ifndef __IVIEW_FACTORY_H__
#define __IVIEW_FACTORY_H__

enum class ViewType
{
    CLS,
    UI,
    WEB
};

class IView;
class IViewFactory
{
public:
    IViewFactory() = default;
    virtual ~IViewFactory() = default;

    virtual IView* CreateView(const ViewType&) = 0;
};

#endif //__IVIEW_FACTORY_H__
