#pragma once

#include "cocos2d.h"
#include "GameSprite.h"

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

	cocos2d::Size m_screenSize;

	cocos2d::Vector<Sprite*> m_wallTiles;
	cocos2d::Vector<Sprite*> m_roofTiles;
	cocos2d::Vector<Sprite*> m_chimneys;

	cocos2d::SpriteFrame * m_tile1;
	cocos2d::SpriteFrame * m_tile2;
	cocos2d::SpriteFrame * m_tile3;
	cocos2d::SpriteFrame * m_tile4;
	int m_tileWidth;
	int m_tileHeight;

	cocos2d::SpriteFrame * m_roof1;
	cocos2d::SpriteFrame * m_roof2;

	cocos2d::Action * m_steamAnimation;
	cocos2d::Action * m_steamSpawn;
	cocos2d::Action * m_steamMove;
	cocos2d::Action * m_steamFade;
	cocos2d::Action * m_steamScale;
	int m_steamIndex;
};