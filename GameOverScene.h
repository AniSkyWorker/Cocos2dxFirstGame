#pragma once
#include "cocos2d.h"

class GameOver :public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void exitPause(cocos2d::Ref* pSender);

	CREATE_FUNC(GameOver);
};

