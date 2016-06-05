#include "Block.h"

USING_NS_CC;

namespace
{
	const int c_steamCount = 3;
	const int c_wTileCount = 8;
	const int c_hTileCount = 6;
}

Block::~Block()
{
    m_chimneys.clear();
    m_wallTiles.clear();
    m_roofTiles.clear();
    
    CC_SAFE_RELEASE(m_steamAnimation);
    CC_SAFE_RELEASE(m_steamSpawn);
    CC_SAFE_RELEASE(m_steamMove);
    CC_SAFE_RELEASE(m_steamFade);
    CC_SAFE_RELEASE(m_steamScale);
}

Block::Block()
	: m_wallTiles(20)
	, m_roofTiles(5)
	, m_chimneys(5)
	, m_puffing(false)
	, m_screenSize(Director::getInstance()->getWinSize())
	, m_tileWidth(m_screenSize.width / c_wTileCount)
	, m_tileHeight(m_screenSize.height / c_hTileCount)
{
    setVisible(false);
}

Block* Block::Create() 
{
    auto block = new Block();
	if (block && block->initWithSpriteFrameName("blank.png"))
	{
		block->autorelease();
        block->InitBlock();
		return block;
	}
	CC_SAFE_DELETE(block);
	return nullptr;
}

void Block::SetPuffing(bool value) 
{
    m_puffing = value;

    if (value) 
	{
        runAction(m_steamSpawn->clone());
        auto hide = Sequence::create(DelayTime::create(2.5f), CallFunc::create(std::bind(&Block::HidePuffs, this)), nullptr);
		runAction(hide);
    } 
	else 
	{
        m_steamIndex = 0;
        for (auto chimney : m_chimneys)
        {
            for (int j = 0; j < c_steamCount; j++)
			{
                auto steam = chimney->getChildByTag(j);
				steam->setVisible(false);
				steam->stopAllActions();
            }
        }
    }
}

void Block::HidePuffs()
{
    SetPuffing(false);
}


int Block::GetLeft() const
{
	return _position.x;
}

int Block::GetRight() const
{
	return _position.x + m_width;
}

int Block::GetTop() const
{
	return m_height;
}

void Block::SetupBlock(int width, int height, BlockType type)
{
    SetPuffing(false);
    m_type = type;
    m_width = width * m_tileWidth;
    m_height = m_tileHeight * (height + 0.5f);
    setPositionY(m_height);
    
    SpriteFrame * wallFrame;
    SpriteFrame * roofFrame = rand() % 100 > 60 ? m_roof1 : m_roof2;
   
	std::vector<float> chimneys;
    
    switch (type) 
	{
	case BlockType::Empty:
        setVisible(false);
        return;
           
	case BlockType::First:
        wallFrame = m_tile1;
		chimneys.push_back(0.2f);
		chimneys.push_back(0.8f);
        break;

	case BlockType::Second:
        wallFrame = m_tile2;
		chimneys.push_back(0.2f);
		chimneys.push_back(0.8f);
		chimneys.push_back(0.5f);
        break;

	case BlockType::Third:
        wallFrame = m_tile3;
		chimneys.push_back(0.2f);
		chimneys.push_back(0.8f);
		chimneys.push_back(0.5f);
        break;

	case BlockType::Forth:
        wallFrame = m_tile4;
		chimneys.push_back(0.2f);
		chimneys.push_back(0.5f);
        break;
    }
    
	
    for (ssize_t i = 0; i < m_chimneys.size(); i++) 
	{
    	auto chimney = m_chimneys.at(i);
        if (static_cast<size_t>(i) < chimneys.size())
		{
            chimney->setPosition(Vec2(chimneys[i] * m_width, 0));
            chimney->setVisible(true);
        }
		else 
		{
            chimney->setVisible(false);
        }
    }
	auto c = rand() % chimneys.size();
	setVisible(true);
    
    for (auto tile : m_roofTiles)
	{
        if (tile->getPositionX() < m_width)
		{
            tile->setVisible(true);
            tile->setSpriteFrame(roofFrame);
        }
		else 
		{
            tile->setVisible(false);
        }
    }
    
    for (auto tile : m_wallTiles) 
	{
        if (tile->getPositionX() < m_width && tile->getPositionY() > -m_height)
		{
            tile->setVisible(true);
            tile->setSpriteFrame(wallFrame);
        }
		else 
		{
            tile->setVisible(false);
        }
    }
}

void Block::InitBlock() 
{
    m_tile1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("building_1.png");
    m_tile2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("building_2.png");
    m_tile3 = SpriteFrameCache::getInstance()->getSpriteFrameByName("building_3.png");
    m_tile4 = SpriteFrameCache::getInstance()->getSpriteFrameByName("building_4.png");
    
    m_roof1 = SpriteFrameCache::getInstance()->getSpriteFrameByName("roof_1.png");
    m_roof2 = SpriteFrameCache::getInstance()->getSpriteFrameByName("roof_2.png");
    
    for (int i = 0; i < 5; i++) 
	{
        auto tile = Sprite::createWithSpriteFrameName("roof_1.png");
        tile->setAnchorPoint(Vec2(0, 1));
        tile->setPosition(Vec2(i * m_tileWidth, 0));
        tile->setVisible(false);
        addChild(tile, LayerType::Front, TileType::RoofTile);
        m_roofTiles.pushBack(tile);
        
        for (int j = 0; j < 4; j++)
		{
            tile = Sprite::createWithSpriteFrameName("building_1.png");
            tile->setAnchorPoint(Vec2(0, 1));
            tile->setPosition(Vec2(i * m_tileWidth, -1 * (m_tileHeight * 0.47f + j * m_tileHeight)));
            tile->setVisible(false);
			addChild(tile, LayerType::Front, TileType::WallTile);
            m_wallTiles.pushBack(tile);
        }
        
    }
       
    for (int i = 0; i < 5; i++)
	{
        auto chimney = Sprite::createWithSpriteFrameName("chimney.png");
        chimney->setVisible(false);
        this->addChild(chimney, LayerType::Front, TileType::Chimney);
        m_chimneys.pushBack(chimney);
        
        for (int j = 0; j < c_steamCount; j++)
		{
            auto steam = Sprite::createWithSpriteFrameName("puff_1.png");
			steam->setAnchorPoint(Vec2(0,-0.5));
			steam->setVisible(false);
            chimney->addChild(steam, -1, j);
        }
        
    }
    
    Animation* animation;
    animation = Animation::create();
    SpriteFrame* frame;

    for(int i = 1; i <= 4; i++) 
	{
        frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("puff_%i.png", i)->getCString());
        animation->addSpriteFrame(frame);
    }

    animation->setDelayPerUnit(0.75f / 4.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    m_steamAnimation = Animate::create(animation);
    m_steamAnimation->retain();

	m_steamMove = MoveBy::create(1.0f, Vec2(-100, 80));
	m_steamMove->retain();
	m_steamFade = FadeOut::create(2.0f);
	m_steamFade->retain();
	m_steamScale = ScaleBy::create(1.5f, 1.5);
	m_steamScale->retain();

    m_steamSpawn = Repeat::create(Sequence::create(DelayTime::create(0.5f), CallFunc::create(std::bind(&Block::CreateSteam, this)), nullptr), c_steamCount);
    m_steamSpawn->retain();
    
    m_steamIndex = 0;
}

void Block::CreateSteam() 
{  
    for ( auto chimney : m_chimneys)
	{
        if (chimney->isVisible())
		{
            auto steam = chimney->getChildByTag(m_steamIndex);
			steam->setVisible(true);
			steam->stopAllActions();
			steam->setScale(1.0);
			steam->setOpacity(255);
			steam->setPosition(Vec2(0,0));
			steam->runAction(m_steamAnimation->clone());
			steam->runAction(m_steamMove->clone());
			steam->runAction(m_steamScale->clone());
        }
    }
    
    m_steamIndex++;
	m_steamIndex = (m_steamIndex == c_steamCount)? 0 : m_steamIndex;
}
