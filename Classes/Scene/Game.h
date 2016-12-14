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
#include "ui/CocosGUI.h"

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
        Z_LABEL,
    };
    
    enum tag{
        T_BULLET = 1,
        T_ENEMY,
        T_PLAYER,
        T_PAUSE,
    };
    
    //衝突判定用
    enum category{
        C_PLAYER = 1, //0001
        C_ENEMY,      //0010
    };
    
    struct EnemyConfig{
        int enemyType;
        int itemType;
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
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    
    //衝突イベント
    bool onContactBegin(cocos2d::PhysicsContact& object);
    
    void pause();
    void unPause();
    
private:
    cocos2d::PhysicsWorld* sceneWorld;
    cocos2d::Vec2 origin;
    cocos2d::Size visibleSize;
    cocos2d::Label* mScoreLabel;
    cocos2d::Sprite* backGroundA;
    cocos2d::Sprite* backGroundB;
    std::string mScoreString;
    
    void goToGameOver();
    void goToPause();
    
    void setPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };
    void initPhysics();
    void initTouchEvent();
    void initBackGround();
    void initPauseButton();
    void initScoreLabel();
    
    void initPlayer();
    void initStageData();
    void spawnEnemy();
    void spawnPlayerBullet();
    void spawnEnemyBullet();
    void updateBackGround();

    void update(float dt);
    
    Player* mPlayer;
    bool mTouchFlag; //Playerがタッチされているかどうか
    std::vector<EnemyConfig> mEnemyConfig;
    float mTime;
    int mScore;
    int mBgMoveSpeed; //背景画像移動速度（ピクセル）
};

#endif // __GAME_SCENE_H__
