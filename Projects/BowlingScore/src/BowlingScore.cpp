#include "Lane.h"
#include <memory>

int main()
{
    std::unique_ptr<ILane> lane(new Lane);
    lane->Init({"Peter" ,  "John", "Sarah"});
    lane->Play([&lane]{
        lane->Finish();
    });
    
    return 0;
}
