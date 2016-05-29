#pragma once
#define TILE_H_SIZE 6
#define TILE_W_SIZE 8

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;


enum {
    kWallTile,
    kRoofTile,
    kChimney
};

enum class BlockType 
{
    Empty,
    First,
    Second,
    Third,
    Forth
};

class Block : public GameSprite {
    
    Size m_screenSize;
    
    Vector<Sprite*> _wallTiles;
    Vector<Sprite*> _roofTiles;
    
    SpriteFrame * _tile1;
    SpriteFrame * _tile2;
    SpriteFrame * _tile3;
    SpriteFrame * _tile4;
    
    SpriteFrame * _roof1;
    SpriteFrame * _roof2;
    
    Action * _puffAnimation;
    Action * _puffSpawn;
    Action * _puffMove;
    Action * _puffFade;
    Action * _puffScale;
    
    
    int m_tileWidth;
    int m_tileHeight;
    int _puffIndex;
    
    
    void initBlock(void);
    void createPuff(void);
    
public:
    
    CC_SYNTHESIZE(BlockType, m_type, Type);
    CC_SYNTHESIZE_READONLY(bool, _puffing, Puffing);
	CC_SYNTHESIZE(Vector<Sprite*>, _chimneys, Chimneys);
    
    
    Block();
    virtual ~Block();
    
    static Block * create();
    void SetupBlock(int width, int height, BlockType type);
    void setPuffing (bool value);
    void hidePuffs();
    
    
    inline virtual int left() {
    	return this->getPositionX();
	}
    
	inline virtual int right() {
    	return this->getPositionX() + m_width;
	}
    
    inline virtual int top() {
        return this->getHeight();
    }
    
    inline virtual int bottom() {
		return 0;
    }
    
    
};