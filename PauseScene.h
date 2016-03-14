#pragma once
#include "cocos2d.h"

class Pause : public cocos2d::Layer
{
public:
	static cocos2d::Scene* CreateScene();
	virtual bool init();

	void InitMenu();
	void InitLabels();
	void ExitPause(cocos2d::Ref* pSender);

	CREATE_FUNC(Pause);

private:
	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;
};