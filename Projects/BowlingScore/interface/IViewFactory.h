#ifndef __IVIEW_FACTORY_H__
#define __IVIEW_FACTORY_H__

#include <memory>

enum class ViewType
{
    CLS,
    TEXT,
    UI,
    WEB
};

class IView;
class IViewFactory
{
public:
    IViewFactory() = default;
    virtual ~IViewFactory() = default;

    virtual std::unique_ptr<IView> CreateView(const ViewType&) = 0;
};

#endif //__IVIEW_FACTORY_H__

