//
//  Game.h
//  CatchGame
//
//  Created by rinon on 2016/12/07.
//
//

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

class Player;
class Enemy;

class Game : public cocos2d::Layer
{
public:
    
    enum z_order{
        Z_BG,
        Z_BULLET,
        Z_ENEMY,
        Z_PLAYER,
        Z_BUTTON,
    };
    
    enum tag{
        T_BULLET = 1,
        T_ENEMY,
        T_PLAYER,
    };
    
    //衝突判定用
    enum category{
        C_PLAYER = 1, //0001
        C_ENEMY,      //0010
    };
    
    struct EnemyConfig{
        int enemyType;
        int spawnTime;
    };
    
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(Game);
    
    //タップイベント
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    
    //衝突イベント
    bool onContactBegin(cocos2d::PhysicsContact& object);
    
private:
    cocos2d::PhysicsWorld* sceneWorld;
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    
    void goToGameOver();
    void goToPause();
    
    void setPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
    void initPhysics();
    void initTouchEvent();
    void initPauseButton();
    
    void initPlayer();
    void initEnemy();
    void spawnEnemy();
    void spawnPlayerBullet();
    void spawnEnemyBullet();
    
   // void CollisionDetection();
    void update(float dt);
    
    Player* mPlayer;
    std::vector<EnemyConfig> mEnemyConfig;
    float mTime;
};

#endif // __GAME_SCENE_H__
