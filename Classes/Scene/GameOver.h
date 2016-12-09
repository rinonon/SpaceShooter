//
//  Title.h
//  CatchGame
//
//  Created by rinon on 2016/12/06.
//
//

#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__

#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameOver);
    
private:
    cocos2d::Menu* menu;
    cocos2d::Node* titleLabel;
    cocos2d::Node* playLabel;
    cocos2d::Node* settingLabel;
    
    void NextGame(Ref* pSender);
    void NextTitle(Ref* pSender);
};

#endif // __GAMEOVER_SCENE_H__
