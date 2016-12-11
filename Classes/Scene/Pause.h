//
//  Pause.h
//  CatchGame
//
//  Created by rinon on 2016/12/06.
//
//

#ifndef __PAUSE_SCENE_H__
#define __PAUSE_SCENE_H__

#include "cocos2d.h"

class Pause : public cocos2d::Layer
{
public:
    static cocos2d::Layer* createLayer();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Pause);
    
private:
    cocos2d::Menu* menu;
    
    void goToGame(Ref* pSender);
    void goToTitle(Ref* pSender);
    void goToRestart(Ref* pSender);
};

#endif // __PAUSE_SCENE_H__
