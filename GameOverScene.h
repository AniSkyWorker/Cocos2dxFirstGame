#pragma once
#include "cocos2d.h"

class GameOver : public cocos2d::Layer
{
public:
	static cocos2d::Scene* CreateScene();
	virtual bool init() override;

	void InitMenu();
	void InitLabels();
	void InitScore();
	void ExitPause(cocos2d::Ref* pSender);

	CREATE_FUNC(GameOver);

private:
	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;
};

