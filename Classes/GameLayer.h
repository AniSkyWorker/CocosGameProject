#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Area.h"
#include "Player.h"

enum class GameState 
{
    Intro,
    Play,
    Over,
	Tutorial,
	TutorialJump,
    TutorialFloat,
    TutorialDrop
};

class GameLayer : public cocos2d::Layer
{
public:
    virtual ~GameLayer();
    virtual bool init() override;
    static Scene* scene();
    
    CREATE_FUNC(GameLayer);
    void update(float dt);
    
	bool OnTouchBegan(Touch* touch, Event* event);
    void OnTouchEnded(Touch* touch, Event* event);
    
    void ShowTutorial(cocos2d::Ref* pSender);
    void StartGame(cocos2d::Ref* pSender);

private:
	void CreateGameScreen();
	void CreateFilling();
	void CreateGameObjects();
	void CreateMenu();

	void InitListener();
	void InitCyclistsAnimation();

	void ResetGame();
	void GameOver();

	void UpdateSpritesPlacement();

	void UpdateParallax();
	void Update—yclists();

	void UpdateScore(float dt);
	void IncreaseComplexity(float dt);

	void UpdateTutorial();

	Area* m_area;
	Player* m_player;

	cocos2d::Label* m_scoreDisplay;

	cocos2d::Sprite* m_introSprite;
	cocos2d::Sprite* m_tryAgainLabel;
	cocos2d::Sprite* m_background;
	cocos2d::Sprite* m_foreground;
	cocos2d::Sprite* m_playerHat;
	cocos2d::Sprite* m_cyclists;
	cocos2d::Action* m_cyclistsAnimation;
	cocos2d::Action* m_cyclistsMoving;

	cocos2d::Vector<Sprite*> m_clouds;

	cocos2d::SpriteBatchNode* m_mainBatchNode;
	cocos2d::Label* m_tutorialLabel;
	cocos2d::Menu* m_mainMenu;

	cocos2d::Size m_screenSize;

	GameState m_gameState;

	bool m_isRunning;
	float m_score;
	int m_speedIncreaseInterval;
	float m_speedIncreaseTimer;
};


