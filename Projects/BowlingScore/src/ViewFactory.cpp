#include "ViewFactory.h"
#include "ConsoleView.h"
#include "PlainTextView.h"
#ifdef BOWLING_WITH_QT
#include "QtScoreView.h"
#endif

#include <stdexcept>

ViewFactory::ViewFactory()
{
    //
}

ViewFactory::~ViewFactory()
{
    //
}

std::unique_ptr<IView> ViewFactory::CreateView(const ViewType& type)
{
    switch (type)
    {
        case ViewType::CLS:
            return std::make_unique<ConsoleView>();
        case ViewType::TEXT:
            return std::make_unique<PlainTextView>();
        case ViewType::UI:
#ifdef BOWLING_WITH_QT
            return std::make_unique<QtScoreView>();
#else
            throw std::runtime_error("Qt/QML view is unavailable because BowlingScore was built without Qt6");
#endif
        case ViewType::WEB:
            //TODO: return the proper instance of class here when it will be implemented
            throw std::runtime_error("Unimplemented type of View");
        default:
            throw std::out_of_range("Unknown type of View");
    }
    
    return {};
}

