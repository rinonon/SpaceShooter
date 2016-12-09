//
//  Player.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#include "Game/Player.h"

USING_NS_CC;

bool Player::init(){
    
    if(!Sprite::initWithFile("Player.png")){
        return false;
    }
    
    this->setScale(1.2);
    mHitPoint = 5;
    mBulletInterval = 0.5;
    mLastBulletTime = 0;
    
    mType = TYPE_NORMAL;
    
    return true;
}

void Player::damaged(){
    
    Action* action;
    mHitPoint--;
    
    if(mHitPoint < 0) {
        
        this->stopAllActions();
        action = ScaleTo::create(2, 0.1);
        
        this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create([this]() { this->mType = TYPE_NONE; } ), NULL));
    }
    
    else {
        //死んでいなければ点滅
        action = Blink::create(0.2, 2);
      
    }
    
      this->runAction(action);
}


bool Player::isDead() const {
    
    if(mType == TYPE_NONE){
        return true;
    }
    
    return false;
}


float Player::getBulletInterval() const {
    return mBulletInterval;
}

float Player::getLastBulletTime() const {
    return mLastBulletTime;
}

void Player::setLastBulletTime(float t) {
    mLastBulletTime = t;
}
