#pragma once

#include "cocos2d.h"
enum 
{
    kSpritePlayer,
    kSpriteTerrain,
    kSpriteBlock,
    kSpriteChimney,
    kSpritePuff
};

enum LayerType
{
	Back,
	Middle,
	Front
};

class GameSprite : public cocos2d::Sprite
{
public:
	GameSprite() = default;
	virtual ~GameSprite() = default;

	CC_SYNTHESIZE(cocos2d::Point, m_expectedPosition, ExpectedPosition);
	CC_SYNTHESIZE(float, m_width, Width);
	CC_SYNTHESIZE(float, m_height, Height);
	CC_SYNTHESIZE(cocos2d::Point, m_velocityVec, Velocity);       
};
