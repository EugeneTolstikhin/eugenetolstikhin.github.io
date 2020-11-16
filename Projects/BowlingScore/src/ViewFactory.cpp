#include "ViewFactory.h"
#include "ConsoleView.h"

#include <stdexcept>

ViewFactory::ViewFactory()
{
    //
}

ViewFactory::~ViewFactory()
{
    //
}

IView* ViewFactory::CreateView(const ViewType& type) const
{
    switch (type)
    {
        case ViewType::CLS:
            return new ConsoleView;
        case ViewType::UI:
        case ViewType::WEB:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of View");
        default:
            throw std::out_of_range("Unknown type of View");
    }
    
    return nullptr;
}
