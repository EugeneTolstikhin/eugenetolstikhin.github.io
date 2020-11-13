#include "Lane.h"
#include <memory>

int main()
{
    try
    {
        std::unique_ptr<ILane> lane(new Lane);
        lane->Init({"Peter", "John", "Sarah"});
        lane->Play([&lane]{
            lane->Finish();
        });
    }
    catch(const std::exception& e)
    {
        //TODO: Log the error here
        return 1;
    }
    
    return 0;
}
