//
//  Game.cpp
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//
#include <cmath>
#include <sstream>
#include "ui/CocosGUI.h"

#include "Definition.h"
#include "File.h"
#include "Scene/Game.h"
#include "Scene/Title.h"
#include "Scene/GameOver.h"
#include "Scene/Pause.h"

#include "Game/Bullet.h"
#include "Game/Player.h"
#include "Game/Enemy.h"

USING_NS_CC;

Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
  
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
    
    //パラメータ初期設定
    mScore = 0;
    mTime = 0;
    mBgMoveSpeed = 4;
    
    initBackGround();
    
    initPauseButton();
    
    initScoreLabel();
    
    initPlayer();
    
    initTouchEvent();
    
    initPhysics();
    
    initStageData();
    
    scheduleUpdate();
    
    return true;
}


void Game::initBackGround() {
    
    backGroundA = Sprite::create("Space.png");
    backGroundA->setAnchorPoint(Vec2(0,0));
    backGroundA->setPosition(Vec2(0 + origin.x, 0));
    this->addChild(backGroundA, Z_BG);
    
    //Aの下に設置
    backGroundB = Sprite::create("Space.png");
    backGroundB->setAnchorPoint(Vec2(0,0));
    backGroundB->setPosition(Vec2(0, -backGroundB->getContentSize().height));
    this->addChild(backGroundB, Z_BG);
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

void Game::initScoreLabel(){
    mScoreLabel = Label::createWithSystemFont("Score", "Arial", 12);
    mScoreLabel->setColor(Color3B(255, 255, 255));
    mScoreLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height - mScoreLabel->getContentSize().height ));
    
    this->addChild(mScoreLabel, Z_LABEL);
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
          //  log("player's bullet and enemy");
            Enemy* enemy = static_cast<Enemy*>(nodeB);
            
            bullet->hit();
            mScore += enemy->damaged(bullet->getPower());
        }
        
        //プレイヤーと敵の弾
        else if(nodeB->getTag() == T_PLAYER) {
            Player* player = static_cast<Player*>(nodeB);
            
            bullet->hit();
            player->damaged();
        }
    }
    
    //弾との衝突 ShapeB:Bullet
    else if(nodeB->getTag() == T_BULLET){
        
        Bullet* bullet = static_cast<Bullet*>(nodeB);
        
        //敵とプレイヤーの弾
        if(nodeA->getTag() == T_ENEMY){
           // log("player's bullet and enemy");
            Enemy* enemy = static_cast<Enemy*>(nodeA);
            
            bullet->hit();
            mScore += enemy->damaged(bullet->getPower());
        }
        
        //プレイヤーと敵の弾
        else if(nodeA->getTag() == T_PLAYER) {
            Player* player = static_cast<Player*>(nodeA);
            
            bullet->hit();
            player->damaged();
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


void Game::initStageData(){
    mEnemyConfig = {
        {0, 3}, {0, 5}, {0, 7}, {1, 8}, {0, 12},
        {1, 15}, {0, 19}, {1, 22}, {1, 25}, {0, 30},
        {0, 37}, {1, 37}, {0, 37}, {0, 40}, {0, 40},
        {0, 40}, {1, 45}, {1, 46}, {1, 47}, {1, 48},
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
        double angle = atan(((mPlayer->getPositionY() - enemy->getPositionY()) ) / ((mPlayer->getPositionX() - enemy->getPositionX() )));
        
        //Playerのほうが左
        if(mPlayer->getPositionX() < enemy->getPositionX()){
            angle += M_PI;
        }
        
        if(angle >= 0 && angle < M_PI_2) {
            angle = 0;
            
        } else if(angle <= M_PI && angle > M_PI_2) {
            angle = M_PI;
        }
                
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
    
    //プレイヤーが死んだらゲームオーバーへ
    if(mPlayer->isDead()){
        goToGameOver();
    }
    
    //時間になったらエネミー生成
    if(mEnemyConfig.size() > 0 && mTime >= mEnemyConfig[0].spawnTime){
        spawnEnemy();
    }
    
    spawnPlayerBullet();
    
    spawnEnemyBullet();
    
    //Score表示
    mScoreString = "score: " + std::to_string(mScore);
    mScoreLabel->setString(mScoreString);
    
    //背景処理
    updateBackGround();
    
    mTime += dt;

}

void Game::goToGameOver(){
    
    auto scene = GameOver::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void Game::goToPause(){

    //一時停止し、新たにレイヤーを重ねる
    pause();
    auto layer = Pause::createLayer();
    this->addChild(layer, T_PAUSE);
}


void Game::pause(){
 
    this->cocos2d::Node::pause();
    
    for(auto node : this->getChildren()) {
        node->pause();
    }
}


void Game::unPause(){

    this->cocos2d::Node::resume();
    mPlayer->resume();
    
    for(auto node : this->getChildren()) {
        node->resume();
    }
}


void Game::updateBackGround() {
    
    // 背景ABを毎フレームごとに上にスクロールする
    backGroundA->setPositionY(backGroundA->getPositionY() + mBgMoveSpeed);
    backGroundB->setPositionY(backGroundB->getPositionY() + mBgMoveSpeed);

    //Aが画面外にでたらBの下に移動
    if(backGroundA->getPositionY() > visibleSize.height){
        
        backGroundA->setPositionY(backGroundB->getPositionY() - backGroundA->getContentSize().height);
        
        //AとBを入れ替える
        auto temp = backGroundB;
        backGroundB = backGroundA;
        backGroundA = temp;
    }
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
