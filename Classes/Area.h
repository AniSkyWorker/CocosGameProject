#pragma once

#include "cocos2d.h"
#include "Block.h"
#include "Player.h"
#include "BlockData.h"

class Area : public cocos2d::Sprite
{	
public:
	Area();
	virtual ~Area();
	
    static Area* Create();
	void ActivateChimneys(Player* player);
    void СheckCollision(Player* player);
	
	void Move(float xMove);
    void Reset();

	CC_SYNTHESIZE(bool, m_startGame, StartGame);

private:
	void InitTerrain();
	void AddBlocks(int currentWidth);

	void SetBlocksPositions();
	void InitBlock(Block* block);

	int GetAreaWidth() const;

	cocos2d::Vector<Block*> m_blockPool;
	size_t m_currentPoolIndex;

	cocos2d::Vector<Block*> m_currentArea;
	int m_lastBlockHeight;
	int m_lastBlockWidth;
	int m_currentMinWidth;

	cocos2d::Size m_screenSize;
	bool m_showEmpty;

	int m_currentKitIndex;
	int m_currentKitCounter;

	int m_currentWidthIndex;
	int m_currentHeightIndex;
	int m_currentTypeIndex;

	float m_emptyInterval;
	int m_currentEmptySize;

	SCombinationsBlockData m_blockData;
};
    