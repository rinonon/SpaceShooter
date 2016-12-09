//
//  Title.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/06.
//
//

#include "Scene/Title.h"
#include "Scene/Game.h"
#include "Definition.h"

USING_NS_CC;

Scene* Title::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Title::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Title::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    Sprite* backGround = Sprite::create("background.jpg");
    //backGround->setAnchorPoint(Vec2(0,0));
    backGround->setPosition(Vec2(visibleSize.width + origin.x, visibleSize.height + origin.y));
    this->addChild(backGround, -1);
    
    //TitleLabel
    auto  titleLabel = Label::createWithSystemFont("Game Title", "Arial", 24);
    titleLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, ((visibleSize.height/4)*3) + origin.y));
   
    //Menu
    auto playLabel = Label::createWithSystemFont("Play Game", "Arial", 12);
    auto menu_item1 = MenuItemLabel::create(playLabel, CC_CALLBACK_1(Title::NextGame, this));
    auto menu = Menu::create(menu_item1, NULL);
    menu->alignItemsVertically();
    
    menu->setPosition(Vec2((visibleSize.width)/2 + origin.x, (visibleSize.height)/3*1 + origin.y));
   
    this->addChild(titleLabel);
    this->addChild(menu);
    return true;
    
}

void Title::NextGame(cocos2d::Ref* pSender){
    
    auto scene = Game::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}


void Title::menuCloseCallback(Ref* pSender)
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
