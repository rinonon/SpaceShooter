//
//  Title.h
//  CatchGame
//
//  Created by rinon on 2016/12/06.
//
//

#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

class Title : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Title);
    
    
private:
    void NextGame(Ref* pSender);
};

#endif // __TITLE_SCENE_H__
