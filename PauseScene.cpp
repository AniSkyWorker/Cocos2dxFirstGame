#include "PauseScene.h"

USING_NS_CC;

Scene* Pause::CreateScene()
{
	auto scene = Scene::create();
	auto layer = Pause::create();
	scene->addChild(layer);

	return scene;
}

bool Pause::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	InitMenu();
	InitLabels();

	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2());
	bg->setPosition(0, 0);
	this->addChild(bg, -1);

	return true;
}

void Pause::ExitPause(cocos2d::Ref* pSender)
{
	Director::getInstance()->popScene();
}

void Pause::InitMenu()
{
	auto pauseItem = MenuItemImage::create("play.png", "play_pressed.png",
		CC_CALLBACK_1(Pause::ExitPause, this));
	pauseItem->setPosition(Vec2(m_origin.x + m_visibleSize.width - pauseItem->getContentSize().width / 2,
		m_origin.y + pauseItem->getContentSize().height / 2 + 100));

	auto menu = Menu::create(pauseItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void Pause::InitLabels()
{
	auto label = Label::createWithTTF("PAUSE", "fonts/Marker Felt.ttf", 96);
	label->setPosition(m_origin.x + m_visibleSize.width / 2,
		m_origin.y + m_visibleSize.height / 2);
	this->addChild(label, 1);
}