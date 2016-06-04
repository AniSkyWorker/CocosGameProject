#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

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
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if(!glview)
	{
		glview = GLViewImpl::create("RainDogs");
		director->setOpenGLView(glview);
	}
	director->setDisplayStats(true);
	director->setAnimationInterval(1.0f / 60);
	Size screenSize = glview->getFrameSize();
	Size designSize = Size(2048, 1536);
	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);
	
	std::vector<std::string> searchPaths;
	if (screenSize.height > 768)
	{
		searchPaths.push_back("ipadhd");
		director->setContentScaleFactor(1536/designSize.height);
	}
	else if (screenSize.height > 320)
	{
		searchPaths.push_back("ipad");
		director->setContentScaleFactor(768/designSize.height);
	}
	else
	{
		searchPaths.push_back("iphone");
		director->setContentScaleFactor(380/designSize.height);
	}
	searchPaths.push_back("res");
	auto fileUtils = FileUtils::getInstance();
	fileUtils->setSearchPaths(searchPaths);
	
	auto audioEngine = SimpleAudioEngine::getInstance();
	audioEngine->preloadBackgroundMusic("1.mp3");
	audioEngine->preloadEffect("falling.wav");
	audioEngine->preloadEffect("cat.mp3");
	audioEngine->preloadEffect("hitBuilding.wav");
	audioEngine->preloadEffect("jump.wav");
	audioEngine->preloadEffect("crashing.wav");
	audioEngine->preloadEffect("start.wav");
	audioEngine->preloadEffect("openUmbrella.wav");

	audioEngine->setBackgroundMusicVolume(0.5f);
	audioEngine->setEffectsVolume(0.5f);

	auto scene = GameLayer::scene();
	
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
