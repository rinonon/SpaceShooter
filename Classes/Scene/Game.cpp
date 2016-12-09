//
//  Game.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//
#include <cmath>
#include "ui/CocosGUI.h"

#include "Definition.h"
#include "Scene/Game.h"
#include "Scene/Title.h"
#include "Scene/GameOver.h"

#include "Game/Bullet.h"
#include "Game/Player.h"
#include "Game/Enemy.h"

USING_NS_CC;

Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  
    // 'layer' is an autorelease object
    auto layer = Game::create();
    layer->setPhysicsWorld(scene->getPhysicsWorld());
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool Game::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    origin = Director::getInstance()->getVisibleOrigin();
    
    initPauseButton();
    
    initPlayer();
    
    initTouchEvent();
    
    initPhysics();
    
    initEnemy();
    
    scheduleUpdate();
    
    return true;
}


void Game::initPauseButton() {
    
    auto button = cocos2d::ui::Button::create("PauseButton.png");
    button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
        switch(type){
            case cocos2d::ui::Widget::TouchEventType::BEGAN:
                break;
                
            case cocos2d::ui::Widget::TouchEventType::ENDED:
                goToPause();
                break;
                
            default: break;
        }
    });
    
    button->setScale(0.2);
    
    button->setAnchorPoint(Vec2(1,1));
    button->setPosition(Vec2(visibleSize.width + origin.x, visibleSize.height + origin.y));
    
    this->addChild(button, Z_BUTTON);
}

bool Game::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event){
    mPlayer->setPosition(touch->getLocation());
    return true;
}

void Game::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
    mPlayer->setPosition(touch->getLocation());
}

void Game::initTouchEvent(){
    
    //タッチリスナー
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Game::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return;
}

void Game::initPhysics(){
    
    // 物理衝突リスナー
    auto physListener = EventListenerPhysicsContact::create();
    physListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(physListener, this);
}

//衝突判定
bool Game::onContactBegin(cocos2d::PhysicsContact& object){
    
    //log("ShapeA: %d  ShapeB: %d", object.getShapeA()->getCategoryBitmask(), object.getShapeB()->getCollisionBitmask());
    
    if ((object.getShapeA()->getCategoryBitmask() & object.getShapeB()->getCollisionBitmask()) == 0
        || (object.getShapeB()->getCategoryBitmask() & object.getShapeA()->getCollisionBitmask()) == 0)
    {
        // shapes can't collide
        return false;
    }
    
    Node* nodeA = object.getShapeA()->getBody()->getNode();
    Node* nodeB = object.getShapeB()->getBody()->getNode();
    
    
    //弾との衝突 ShapeA:Bullet
    if(nodeA->getTag() == T_BULLET){
        
        Bullet* bullet = static_cast<Bullet*>(nodeA);
        
        //敵とプレイヤーの弾
        if(nodeB->getTag() == T_ENEMY){
            log("player's bullet and enemy");
            Enemy* enemy = static_cast<Enemy*>(nodeB);
            
            bullet->hit();
            enemy->damaged(bullet->getPower());
        }
        
        //プレイヤーと敵の弾
        else if(object.getShapeB()->getTag() == T_PLAYER) {
            
        }
    }
    
    //弾との衝突 ShapeB:Bullet
    else if(nodeB->getTag() == T_BULLET){
        
        Bullet* bullet = static_cast<Bullet*>(nodeB);
        
        //敵とプレイヤーの弾
        if(nodeA->getTag() == T_ENEMY){
            log("player's bullet and enemy");
            Enemy* enemy = static_cast<Enemy*>(nodeA);
            
            bullet->hit();
            enemy->damaged(bullet->getPower());
        }
        
        //プレイヤーと敵の弾
        else if(nodeA->getTag() == T_PLAYER) {
            
        }
    }
    
    //プレイヤーと敵の衝突
    else if(nodeA->getTag() == T_PLAYER) {
        Player* player = static_cast<Player*>(nodeA);
        // Enemy* enemy = static_cast<Enemy*>(nodeB);
        
        player->damaged();
    }
    
    
    return true;
}



//Player haichi
void Game::initPlayer(){
    mPlayer = Player::create();
    mPlayer->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/5 + origin.y));
    
    PhysicsBody* body = PhysicsBody::createCircle(mPlayer->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);
    
    body->setDynamic(false);
    
    body->setCategoryBitmask(static_cast<int>(C_PLAYER)); //カテゴリ設定
    body->setCollisionBitmask(static_cast<int>(C_ENEMY)); //衝突判定カテゴリ設定
    body->setContactTestBitmask(0xffffffff);
    
    mPlayer->setPhysicsBody(body);
    
    this->addChild(mPlayer, Z_PLAYER, T_PLAYER);
}


void Game::initEnemy(){
    mEnemyConfig = {
        {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6},
        {1, 8}, {1, 9}, {1, 10}, {1, 11}, {1, 12}
    };
}


//Enemy生成
void Game::spawnEnemy(){
    Enemy::Type type = static_cast<Enemy::Type>(mEnemyConfig[0].enemyType);
    auto enemy = Enemy::create(type);
    
    PhysicsBody* body = PhysicsBody::createCircle(enemy->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);
    
    body->setDynamic(false);
    body->setCategoryBitmask(static_cast<int>(C_ENEMY)); //カテゴリ設定
    body->setCollisionBitmask(static_cast<int>(C_PLAYER)); //衝突判定カテゴリ設定
    body->setContactTestBitmask(0xffffffff);
    
    enemy->setPhysicsBody(body);
    
    this->addChild(enemy, Z_ENEMY, T_ENEMY);
    mEnemyConfig.erase(mEnemyConfig.begin());
}

//弾生成
void Game::spawnPlayerBullet(){
    
    //時間じゃなければreturn
    if(mPlayer->getBulletInterval() + mPlayer->getLastBulletTime() > mTime){
        return;
    }
    
    auto bullet = Bullet::create(Bullet::O_PLAYER, M_PI_2);
    bullet->setPosition(mPlayer->getPosition());
    
    PhysicsBody* body = PhysicsBody::createCircle(bullet->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);
    
    body->setDynamic(false);
    body->setCategoryBitmask(static_cast<int>(C_PLAYER)); //カテゴリ設定
    body->setCollisionBitmask(static_cast<int>(C_ENEMY)); //衝突判定カテゴリ設定
    body->setContactTestBitmask(0xffffffff);
    
    bullet->setPhysicsBody(body);
    
    this->addChild(bullet, Z_BULLET, T_BULLET);
    
    mPlayer->setLastBulletTime(mTime);
}


void Game::spawnEnemyBullet(){
    
    //Get Enemy Nodes
    auto nodes = getChildren();
    Vector<Enemy*> enemies;
    
    //transration Node to Enemy
    for(auto node: nodes){
        if(node->getTag() == T_ENEMY){
            enemies.pushBack(static_cast<Enemy*>(node));
        }
    }
    
    for(auto enemy : enemies){
        
        //if Dead, next enemy
        if(enemy->isDead()){
            continue;
        }
        
        //if not shoot time, continue
        if(enemy->getBulletInterval() + enemy->getLastBulletTime() > mTime) {
            continue;
        }
        
        //create bullet
        
        //calculate angle
        double angle = atan((mPlayer->getPositionX() - enemy->getPositionX()) / (mPlayer->getPositionY() - enemy->getPositionY()));
        
        auto bullet = Bullet::create(Bullet::O_ENEMY, angle);
        bullet->setPosition(enemy->getPosition());
        
        PhysicsBody* body = PhysicsBody::createCircle(bullet->getContentSize().width / 2, PHYSICSBODY_MATERIAL_DEFAULT);
        
        body->setDynamic(false);
        body->setCategoryBitmask(static_cast<int>(C_ENEMY)); //カテゴリ設定
        body->setCollisionBitmask(static_cast<int>(C_PLAYER)); //衝突判定カテゴリ設定
        body->setContactTestBitmask(0xffffffff);
        
        bullet->setPhysicsBody(body);
        
        this->addChild(bullet, Z_BULLET, T_BULLET);
        
        enemy->setLastBulletTime(mTime);
    }
}


void Game::update(float dt){
    mTime += dt;
    
    //時間になったらエネミー生成
    if(mEnemyConfig.size() > 0 && mTime >= mEnemyConfig[0].spawnTime){
        spawnEnemy();
    }
    
    spawnPlayerBullet();
    
    spawnEnemyBullet();
    
    //プレイヤーが死んだらゲームオーバーへ
    if(mPlayer->isDead()){
        goToGameOver();
    }
    
}

void Game::goToGameOver(){
    
    auto scene = GameOver::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void Game::goToPause(){
    
    //  auto scene =
}

void Game::menuCloseCallback(Ref* pSender)
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
