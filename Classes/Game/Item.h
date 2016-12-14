//
//  Player.h
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#ifndef Item_h
#define Item_h

#include "cocos2d.h"

class Item : public cocos2d::Sprite {
public:
    
    enum Type {
        ITEM_NONE,
        
        ITEM_LEBEL,
        ITEM_BARRIER,
        ITEM_LIFE,
    };
    
    Item(Type type);
    static Item* create(Type type);
    bool init(); //initialize func
    
    void hit();
    int getPower();
  
private:
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    Type mType;
    
    const int ITEM_SPEED = 100;
};


#endif /* Item_h */
