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
    virtual ~IViewFactory() = default;
    virtual IView* CreateView(const ViewType&) const = 0;
};

#endif //__IVIEW_FACTORY_H__
