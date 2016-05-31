#include "GameSprite.h"

USING_NS_CC;

GameSprite::GameSprite(void)
:m_velocityVec(Vec2(0,0))
,m_screenSize(Director::getInstance()->getWinSize())
{
}

GameSprite::~GameSprite(void){}

