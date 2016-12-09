//
//  Player.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#include "Game/Enemy.h"

USING_NS_CC;


Enemy::Enemy(Type type) : mType(type)
{}

Enemy* Enemy::create(Type type){
    auto enemy = new Enemy(type);
    
    enemy->init(); //Initialize
    enemy->autorelease();
    
    return enemy;
}

bool Enemy::init(){
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    std::string filename;
    Vec2 initPos;
    Action* action;
    
    mLastBulletTime = 0;
    
    switch(mType){
        case TYPE1:
            filename = "Enemy1.png";
            mHitPoint = 3;
            mBulletInterval = 2;
            action = getAction1();
            break;
            
        case TYPE2:
            filename = "Enemy2.png";
            mHitPoint = 5;
            mBulletInterval = 1;
            action = getAction2();
            break;
            
        default: break;
    }
    
    if(!Sprite::initWithFile(filename)){
        return false;
    }
    
    float rand = CCRANDOM_0_1();
    
    if(rand > 0.9f){
        rand = 0.9f;
    } else if(rand < 0.1){
        rand = 0.1f;
    }
    
    initPos = Vec2(visibleSize.width * rand + origin.x , visibleSize.height);
    
    this->setScale(2);
    this->setPosition(initPos);
    this->runAction(action);
    
    return true;
}

Action* Enemy::getAction1(){
    
    auto move1 = MoveTo::create(5, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 3 * 2 + origin.y ));
     auto move2 = MoveTo::create(5, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 3 * 1 + origin.y ));
     auto move3 = MoveTo::create(5, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 3 * 0 + origin.y ));
    auto delay = DelayTime::create(3);
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move1, delay, move2, delay->clone(), move3, remove, nullptr);
    
    return seq;
}

Action* Enemy::getAction2(){
    
    auto move1 = MoveTo::create(2, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 4 * 3 + origin.y ));
    auto move2 = MoveTo::create(2, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 4 * 2 + origin.y ));
    auto move3 = MoveTo::create(2, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 4 * 1 + origin.y ));
    auto move4 = MoveTo::create(2, Vec2(visibleSize.width * CCRANDOM_0_1() + origin.x, visibleSize.height / 4 * 0 + origin.y ));
    
    auto delay = DelayTime::create(1);
    auto remove = RemoveSelf::create();
    auto seq = Sequence::create(move1, delay, move2, delay->clone(), move3, delay->clone(), move4, remove, nullptr);
    return seq;
}


void Enemy::damaged(int power) {
    
    mHitPoint -= power;
    Action* action;
    
    if(mHitPoint < 0) {
        
        mType = TYPE_NONE;
        
        this->stopAllActions();
        
        action = RemoveSelf::create();
    } else {
    
        //死んでいなければ点滅
        action = Blink::create(0.2, 2);
    }
    
    this->runAction(action);
}

bool Enemy::isDead() const{
    
    if(mType == TYPE_NONE){
        return true;
    }
    
    return false;
}


float Enemy::getBulletInterval() const {
    return mBulletInterval;
}

void Enemy::setLastBulletTime(float t) {
    mLastBulletTime = t;
}

float Enemy::getLastBulletTime() const {
    return mLastBulletTime;
}
