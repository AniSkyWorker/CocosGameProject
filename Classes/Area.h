#pragma once

#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

#include "cocos2d.h"
#include "Block.h"
#include "Player.h"

USING_NS_CC;
using namespace std;


class Area : public Sprite
{
    Vector<Block *> m_blockPool;
	size_t m_currentPoolIndex;
	
    Vector<Block *> m_currentArea;
	int m_lastBlockHeight;
    int m_lastBlockWidth;
	int m_currentMinWidth;
    
    bool m_showEmpty;
    Size m_screenSize;
    
    int m_currentKitIndex;
    int m_currentKitCounter;
    int m_currentWidthIndex;
    int m_currentHeightIndex;
    int m_currentTypeIndex;
    
    float m_emptyInterval;
    int m_currentEmptySize;
    
    void InitTerrain (void);
    void AddBlocks(int currentWidth);
	
    void SetBlocksPositions();
    void InitBlock(Block * block);
    
    inline int getWidth() 
	{
        int width = 0;
        for (auto block : m_currentArea)
		{
            width += block->getWidth();
        }
        return width;
    }
    
		
public:
	Area();
	virtual ~Area();
	
	CC_SYNTHESIZE(bool, m_startGame, StartGame);
	
    static Area * create();
    
	void activateChimneysAt (Player * player);
    void СheckCollision (Player * player);
	
	void Move (float xMove);
    void Reset (void);
	
};
    