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
#include "ui/CocosGUI.h"

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
    void update();
    float getBulletInterval() const;
    bool isDead() const;
    void setLastBulletTime(float t);
    float getLastBulletTime() const;
    
private:
    float mLastBulletTime;
    float mBulletInterval;
    int mHitPoint;
    int mHpMax;
    Type mType;
    cocos2d::ui::LoadingBar* mHpBar;
    
};


#endif /* Player_h */
