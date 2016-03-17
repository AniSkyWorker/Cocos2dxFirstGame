#include "GameScene.h"
#include "PauseScene.h"
#include "GameOverScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

const int C_BOMBS_COUNT = 3;
const int C_SCORE_INC = 10;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();

	scene->getPhysicsWorld()->setGravity(Vect(0, 0));

	auto layer = GameScene::create();
	scene->addChild(layer);

    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	
	m_score = 0;
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	auto bg = Sprite::create("background.png");
	bg->setAnchorPoint(Vec2::ZERO);
	bg->setPosition(0, 0);
	this->addChild(bg, -1);

	schedule(CC_SCHEDULE_SELECTOR(GameScene::AddBombs), 3.0f);
	schedule(CC_SCHEDULE_SELECTOR(GameScene::IncrementScore), 3.0f);

	InitPhysics();
	InitTouch();
	InitAudio();
	InitPlayer();
	InitMenu();

	return true;
}

void GameScene::PauseCallback(cocos2d::Ref* pSender)
{
	(void)pSender;
	Director::getInstance()->pushScene(TransitionFlipX::create(1.0, Pause::CreateScene()));
}

void GameScene::SetPhysicsBody(cocos2d::Sprite* sprite)
{
	auto body = PhysicsBody::createCircle(sprite->getContentSize().width / 2);

	body->setContactTestBitmask(true);
	body->setDynamic(true);

	sprite->setPhysicsBody(body);
}

bool GameScene::OnCollision(PhysicsContact& contact)
{ 
	auto playerShape = m_playerSpr->getPhysicsBody()->getFirstShape();

	if (playerShape != contact.getShapeA() && playerShape != contact.getShapeB())
	{
		return false;
	}

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playEffect("uh.wav");

	UserDefault::getInstance()->setIntegerForKey("score", m_score);

	_director->replaceScene(TransitionFlipX::create(1.0, GameOver::CreateScene()));

	return false;
}

void GameScene::InitPhysics()
{ 
	auto contactListener = EventListenerPhysicsContact::create();

	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::OnCollision, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::InitTouch()
{
	auto listener = EventListenerTouchOneByOne::create();

	listener->onTouchBegan = CC_CALLBACK_2(GameScene::ExplodeBombs, this);
	listener->onTouchMoved = CC_CALLBACK_2(GameScene::MovePlayerByTouch, this);
	listener->onTouchEnded = [=](Touch* touch, Event* event){};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::MovePlayerIfPossible(float newX)
{
	float sprHalfWidth = m_playerSpr->getBoundingBox().size.width / 2;

	if (newX >= sprHalfWidth && newX < m_visibleSize.width - sprHalfWidth)
	{
		m_playerSpr->setPositionX(newX);
	}
}

void GameScene::MovePlayerByTouch(Touch* touch, Event* event)
{ 
	(void)event;

	auto touchLocation = touch->getLocation();

	if (m_playerSpr->getBoundingBox().containsPoint(touchLocation))
	{ 
		MovePlayerIfPossible(touchLocation.x);
	} 
}

void GameScene::AddBombs(float dt)
{
	(void)dt;

	for (int i = 0; i < C_BOMBS_COUNT; i++)
	{
		Sprite* bomb = Sprite::create("bomb.png");
		
		bomb->setPosition(CCRANDOM_0_1() * m_visibleSize.width, m_visibleSize.height + bomb->getContentSize().height / 2);
		this->addChild(bomb, 1);

		SetPhysicsBody(bomb);
		bomb->getPhysicsBody()->setVelocity(Vect(0, ((CCRANDOM_0_1() + 0.8f) * -250)));

		m_bombs.pushBack(bomb);
	}
}

void GameScene::IncrementScore(float dt)
{
	(void)dt;

	m_score += C_SCORE_INC;
}

void GameScene::InitAudio()
{
	SimpleAudioEngine::getInstance()->playBackgroundMusic("music.mp3", true);
	SimpleAudioEngine::getInstance()->preloadEffect("uh.wav");
	SimpleAudioEngine::getInstance()->preloadEffect("bomb.mp3");
}

bool GameScene::ExplodeBombs(cocos2d::Touch* touch, cocos2d::Event* event)
{
	(void)event;

	Vec2 touchLocation = touch->getLocation();
	cocos2d::Vector<cocos2d::Sprite*> toErase;
	for (auto bomb : m_bombs)
	{
		if (bomb->getBoundingBox().containsPoint(touchLocation))
		{
			auto explosion = ParticleExplosion::create();
			explosion->setDuration(0.25f);

			SimpleAudioEngine::getInstance()->playEffect("bomb.mp3");

			explosion->setTotalParticles(800);
			explosion->setSpeed(3.5f);
			explosion->setLife(300.0f);
			explosion->setPosition(bomb->getPosition());

			this->addChild(explosion);
			bomb->setVisible(false);

			toErase.pushBack(bomb);
			this->removeChild(bomb);
		}
	}

	for (auto bomb : toErase)
	{
		m_bombs.eraseObject(bomb);
	}

	return true;
}

void GameScene::InitMenu()
{
	auto closeItem = MenuItemImage::create("pause.png", "pause_pressed.png",
		CC_CALLBACK_1(GameScene::PauseCallback, this));
	closeItem->setPosition(Vec2(m_visibleSize.width - closeItem->getContentSize().width / 2,
		closeItem->getContentSize().height / 2 + 100.f));

	auto menu = Menu::create(closeItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void GameScene::InitPlayer()
{
	m_playerSpr = Sprite::create("player.png");
	m_playerSpr->setPosition(m_visibleSize.width / 2, m_visibleSize.height * 0.23);
	this->addChild(m_playerSpr, 0);
	SetPhysicsBody(m_playerSpr);

	Vector<SpriteFrame*> frames;
	Size playerSize = m_playerSpr->getContentSize();
	frames.pushBack(SpriteFrame::create("player.png", Rect(0, 0, playerSize.width, playerSize.height)));
	frames.pushBack(SpriteFrame::create("player2.png", Rect(0, 0, playerSize.width, playerSize.height)));
	auto animation = Animation::createWithSpriteFrames(frames, 0.2f);
	auto animate = Animate::create(animation);
	m_playerSpr->runAction(RepeatForever::create(animate));
}