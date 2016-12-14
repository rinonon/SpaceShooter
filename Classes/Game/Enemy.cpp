//
//  Player.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#include "Game/Enemy.h"
#include "Game/Item.h"
#include "Scene/Game.h"

USING_NS_CC;

Enemy::Enemy(Type type, Item::Type itemType) : mType(type), mItemType(itemType)
{}

Enemy* Enemy::create(Type type, Item::Type itemType){
    auto enemy = new Enemy(type, itemType);
    
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
            mHitPointMax = mHitPoint = 3;
            mBulletInterval = 2;
            mScore = 500;
            action = getAction1();
            break;
            
        case TYPE2:
            filename = "Enemy2.png";
            mHitPointMax = mHitPoint = 5;
            mBulletInterval = 1;
            mScore = 1000;
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
    
    this->setPosition(initPos);
    this->runAction(action);
    
    
    //HPBarの設定
    mHpBar = ui::LoadingBar::create("HPBar.png");
    mHpBar->setPercent(100);
    mHpBar->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height));
    
    this->addChild(mHpBar);
    
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


int Enemy::damaged(int power) {
    
    mHitPoint -= power;
    Action* action;
    int score = 0;
    
    mHpBar->setPercent( 100 * static_cast<float>(mHitPoint) / mHitPointMax);
    
    if(mHitPoint <= 0) {  //死んでいれば
        
        mType = TYPE_NONE;
        score = mScore;
        this->stopAllActions();
        action = RemoveSelf::create();
        
        mHpBar->runAction(RemoveSelf::create());
        
    }
    else {  //死んでいなければ
        action = Blink::create(0.2, 2);
    }
    
    this->runAction(action);
    
    return score;
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
