//
//  Player.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#include "Game/Item.h"

USING_NS_CC;

Item::Item(Type type) : mType(type)
{}

Item* Item::create(Type type){
    
    auto item = new Item(type);
    
    item->init(); //Initialize
    item->autorelease();
    
    return item;
}

bool Item::init(){
    
    std::string filename;
    
    switch (mType) {
        case ITEM_LEBEL:
            filename = "PlayerBullet.png";
            break;
            
        case ITEM_BARRIER:
            filename = "EnemyBullet.png";
            break;
            
        case ITEM_LIFE:
            filename = "";
            break;
            
        default:
            break;
    }
    
    if(!Sprite::initWithFile(filename)){
        return false;
    }
    
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    auto move = MoveBy::create(10, Vec2(0, ITEM_SPEED));
    auto remove = RemoveSelf::create();
    
    auto seq = Sequence::create(move, remove, nullptr);
    
    this->runAction(seq);
    
    return true;
}




