#include "GameOverScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* GameOver::CreateScene()
{
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);

	return scene;
}

bool GameOver::init()
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

void GameOver::ExitPause(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(TransitionFlipX::create(1.0, GameScene::createScene()));
}

void GameOver::InitMenu()
{
	auto pauseItem = MenuItemImage::create("play.png", "play_pressed.png",
		CC_CALLBACK_1(GameOver::ExitPause, this));
	pauseItem->setPosition(Vec2(m_origin.x + m_visibleSize.width - pauseItem->getContentSize().width / 2,
		m_origin.y + pauseItem->getContentSize().height / 2 + 100.f));

	auto menu = Menu::create(pauseItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void GameOver::InitLabels()
{
	auto gameOverLabel = Label::createWithTTF("Game Over", "fonts/Marker Felt.ttf", 96);
	gameOverLabel->setPosition(m_origin.x + m_visibleSize.width / 2,
		m_origin.y + m_visibleSize.height / 2);
	gameOverLabel->enableOutline(Color4B(255, 0, 0, 100), 6);
	gameOverLabel->enableGlow(Color4B(255, 0, 0, 255));
	gameOverLabel->enableShadow();
	this->addChild(gameOverLabel, 1);

	auto label = Label::createWithTTF("Your score is", "fonts/Marker Felt.ttf" , 48);
	label->enableOutline(Color4B(255, 0, 0, 100), 6);
	label->enableGlow(Color4B(255, 0, 0, 255));
	label->enableShadow();
	label->setPosition(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height / 2.5);
	this->addChild(label, 1);

	InitScore();
}

void GameOver::InitScore()
{
	char scoreText[32];
	int score = UserDefault::getInstance()->getIntegerForKey("score", 0);
	sprintf(scoreText, "%d", score);
	auto scoreLabel = Label::createWithBMFont("font.fnt", scoreText);
	scoreLabel->setPosition(m_origin.x + m_visibleSize.width / 2, m_origin.y + m_visibleSize.height / 3.5);
	this->addChild(scoreLabel, 1);
}