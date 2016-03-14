#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
	if (!glview)
	{
		glview = GLViewImpl::create("My Game");
		glview->setFrameSize(480, 800);
		director->setOpenGLView(glview);
	}

	Size screenSize = glview->getFrameSize();
	Size designSize(768, 1280);

	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);
    director->setDisplayStats(false);
    director->setAnimationInterval(1.f / 60.f);

    FileUtils::getInstance()->addSearchPath("images");
	FileUtils::getInstance()->addSearchPath("sounds");

    auto scene = GameScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

     SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
