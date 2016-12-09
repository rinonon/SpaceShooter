//
//  Player.h
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#ifndef Bullet_h
#define Bullet_h

#include "cocos2d.h"

class Bullet : public cocos2d::Sprite {
public:
    
    enum Owner {
        O_PLAYER,
        O_ENEMY,
    };
    
    Bullet(Owner o, float angle);
    static Bullet* create(Owner o, float angle);
    bool init(); //initialize func
    
    bool isBroken();
    void hit();
    int getPower();
  
private:
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    bool mBrokenFlag;
    int mPower;
    Owner mOwner;
    float mAngle;
    int mBulletSpeed;
};


#endif /* Bullet_h */
