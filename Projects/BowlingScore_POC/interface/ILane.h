#ifndef __ILANE_H__
#define __ILANE_H__

#include <functional>
#include <vector>

class ILane
{
public:
    ILane() = default;
    virtual ~ILane() = default;

    virtual void Init(const std::vector<std::string>&) = 0;
    virtual void Play(std::function<void()> gameover) = 0;
    virtual void Finish() = 0;
};

#endif // __ILANE_H__
