#pragma once

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

enum class BlockType 
{
    Empty,
    First,
    Second,
    Third,
    Forth
};

class Block : public GameSprite 
{
	enum TileType
	{
		WallTile,
		RoofTile,
		Chimney
	};
    
public:
    Block();
    virtual ~Block();
    
    static Block* Create();
    void SetupBlock(int width, int height, BlockType type);
    void SetPuffing(bool value);
    void HidePuffs();
    
	int GetLeft() const;
	int GetRight() const;
	int GetTop() const;

	CC_SYNTHESIZE(BlockType, m_type, Type);
	CC_SYNTHESIZE_READONLY(bool, m_puffing, Puffing);

private:
	void InitBlock();
	void CreateSteam();

	Size m_screenSize;

	Vector<Sprite*> m_wallTiles;
	Vector<Sprite*> m_roofTiles;
	Vector<Sprite*> m_chimneys;

	SpriteFrame * m_tile1;
	SpriteFrame * m_tile2;
	SpriteFrame * m_tile3;
	SpriteFrame * m_tile4;
	int m_tileWidth;
	int m_tileHeight;

	SpriteFrame * m_roof1;
	SpriteFrame * m_roof2;

	Action * m_steamAnimation;
	Action * m_steamSpawn;
	Action * m_steamMove;
	Action * m_steamFade;
	Action * m_steamScale;
	int m_steamIndex;
};