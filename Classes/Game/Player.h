//
//  Player.h
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#ifndef Player_h
#define Player_h

#include "cocos2d.h"

class Player : public cocos2d::Sprite {
public:
    
    enum Type {
        TYPE_NORMAL,
        TYPE_MUTEKI,
        
        TYPE_NONE,
    };
    
    bool init(); //initialize func
    CREATE_FUNC(Player); //macro of create func
    
    void damaged();
    float getBulletInterval() const;
    bool isDead() const;
    void setLastBulletTime(float t);
    float getLastBulletTime() const;
    
private:
    float mLastBulletTime;
    float mBulletInterval;
    int mHitPoint;
    Type mType;
    
};


#endif /* Player_h */
