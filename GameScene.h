#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    CREATE_FUNC(GameScene);

	void InitPhysics();
	void InitAudio();
	void InitTouch();
	void InitMenu();
	void InitPlayer();

	bool OnCollision(cocos2d::PhysicsContact& contact);
	void SetPhysicsBody(cocos2d::Sprite* sprite);

	void MovePlayerByTouch(cocos2d::Touch* touch, cocos2d::Event* event);
	void MovePlayerIfPossible(float newX);

	void AddBombs(float dt);
	bool ExplodeBombs(cocos2d::Touch* touch, cocos2d::Event* event);

	void IncrementScore(float dt);
	void PauseCallback(cocos2d::Ref* pSender);
		
private:
	cocos2d::Sprite* m_playerSpr;
	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;
	cocos2d::Vector<cocos2d::Sprite*> m_bombs;
	int m_score;
};