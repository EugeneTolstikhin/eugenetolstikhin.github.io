#ifndef __IPLAYER_H__
#define __IPLAYER_H__

class IPlayer
{
public:
    IPlayer() = default;
    virtual ~IPlayer() = default;

    virtual void GetName() const = 0;
    virtual void GetGame() const = 0;
};

#endif // __IPLAYER_H__