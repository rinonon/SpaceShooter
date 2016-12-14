//
//  Player.h
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#ifndef Enemy_h
#define Enemy_h

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Game/Item.h"


class Enemy : public cocos2d::Sprite {
public:
    
    enum Type {
        TYPE1,
        TYPE2,
        
        TYPE_NONE,
    };
    
    Enemy(Type type, Item::Type itemType);
    static Enemy* create(Type type, Item::Type itemType);
    virtual bool init(); //initialize func
    
    bool isDead() const;
    int damaged(int power);
    float getBulletInterval() const;
    void setLastBulletTime(float t);
    float getLastBulletTime() const;
    
private:
    Type mType;
    Item::Type mItemType;
    int mHitPoint;
    int mHitPointMax;
    int mScore;
    float mBulletInterval;
    float mLastBulletTime;
    cocos2d::ui::LoadingBar* mHpBar;
    
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    cocos2d::Action* getAction1();
    cocos2d::Action* getAction2();
    
  };


#endif /* Enemy_h */
