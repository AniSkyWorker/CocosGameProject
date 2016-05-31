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
	CC_SYNTHESIZE(cocos2d::Point, m_nextPosition, NextPosition);
	CC_SYNTHESIZE(float, m_width, Width);
	CC_SYNTHESIZE(float, m_height, Height);
	CC_SYNTHESIZE(cocos2d::Point, m_velocityVec, Vector);
	
	GameSprite(void);
	virtual ~GameSprite(void);
    
    inline virtual void place ()
	{ 
		setPosition(m_nextPosition); 
	};
    
    inline virtual float radius ()
	{
        return m_width * 0.5f;
    }
    
    inline void setSize()
	{
    	m_width = getBoundingBox().size.width;
		m_height = getBoundingBox().size.height;
    }

protected:
	cocos2d::Size m_screenSize;
        
};
