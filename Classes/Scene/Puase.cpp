//
//  Pause.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/06.
//
//

#include "Scene/Pause.h"
#include "Scene/Title.h"
#include "Scene/Game.h"
#include "Definition.h"

USING_NS_CC;

Layer* Pause::createLayer()
{
    // 'layer' is an autorelease object
    auto layer = Pause::create();
    
    // return the scene
    return layer;
}

// on "init" you need to initialize your instance
bool Pause::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto foreGround = LayerColor::create(Color4B(255, 255, 255, 127), visibleSize.width + origin.x, visibleSize.height + origin.y);
    this->addChild(foreGround, -1);
    
    Sprite* backGround = Sprite::create("backGround.png");
    backGround->setAnchorPoint(Vec2(0,0));
    backGround->setPosition(Vec2(0 + origin.x, 0 + origin.y));
   // this->addChild(backGround, -1);
    
    //TitleLabel
    auto titleLabel = Label::createWithSystemFont("Pause", "Arial", 24);
    titleLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, ((visibleSize.height/4)*3) + origin.y));
   
    //Menu
    auto playLabel = Label::createWithSystemFont("Goback game", "Arial", 12);
    auto settingLabel = Label::createWithSystemFont("Title", "Arial", 12);
    auto restartLabel = Label::createWithSystemFont("Restart", "Arial", 12);
    auto menu_item1 = MenuItemLabel::create(playLabel, CC_CALLBACK_1(Pause::goToGame, this));
    auto menu_item2 = MenuItemLabel::create(settingLabel, CC_CALLBACK_1(Pause::goToTitle, this));
    auto menu_item3 = MenuItemLabel::create(restartLabel, CC_CALLBACK_1(Pause::goToRestart, this));
    menu = Menu::create(menu_item1, menu_item3, menu_item2, NULL);
    menu->alignItemsVertically();
    
    menu->setPosition(Vec2((visibleSize.width)/2 + origin.x, (visibleSize.height)/3*1 + origin.y));
   
    this->addChild(titleLabel);
    this->addChild(menu);
    return true;
    
}

void Pause::goToGame(cocos2d::Ref* pSender){
    
    //親はGame
    static_cast<Game*>(this->getParent())->unPause();
    this->runAction(RemoveSelf::create());
}

void Pause::goToTitle(cocos2d::Ref* pSender){

    auto scene = Title::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void Pause::goToRestart(cocos2d::Ref *pSender){
    
    auto scene = Game::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}


void Pause::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}
