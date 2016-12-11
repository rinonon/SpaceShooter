//
//  Player.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#include "Game/Bullet.h"
#include <cmath>

USING_NS_CC;

Bullet::Bullet(Owner o, float angle) : mOwner(o), mAngle(angle)
{}

Bullet* Bullet::create(Owner o, float angle){
    auto bullet = new Bullet(o, angle);
    
    bullet->init(); //Initialize
    bullet->autorelease();
    
    return bullet;
}

bool Bullet::init(){
    
    std::string filename;
    
    switch (mOwner) {
        case O_PLAYER:
            filename = "PlayerBullet.png";
            mBulletSpeed = 1500;
            break;
            
        case O_ENEMY:
            filename = "EnemyBullet.png";
            mBulletSpeed = 800;
            
        default:
            break;
    }
    
    if(!Sprite::initWithFile(filename)){
        return false;
    }
    
    this->setScale(2);
    mBrokenFlag = false;
    mPower = 1;
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto move = MoveBy::create(5, Vec2(cos(mAngle) * mBulletSpeed, sin(mAngle) * mBulletSpeed));
    auto remove = RemoveSelf::create();
    
    auto seq = Sequence::create(move, remove, nullptr);
    
    this->runAction(seq);
    
    return true;
}



bool Bullet::isBroken(){
    return mBrokenFlag;
}


void Bullet::hit(){
    mBrokenFlag = true;
    
    this->stopAllActions();
    
    Action* remove = RemoveSelf::create();
    runAction(remove);
}


int Bullet::getPower(){
    return mPower;
}
