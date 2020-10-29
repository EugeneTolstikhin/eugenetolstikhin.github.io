#ifndef __IPLAYER_H__
#define __IPLAYER_H__

class IPlayer
{
public:
    IPlayer() = default;
    virtual ~IPlayer() = default;

    virtual void Play() = 0;
};

#endif // __IPLAYER_H__