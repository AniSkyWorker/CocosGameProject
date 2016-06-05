#include "Area.h"
#include "SimpleAudioEngine.h"
#include <algorithm>

using namespace CocosDenshion;
USING_NS_CC;

namespace
{
	const int c_increaseEmptyTimeInterval = 5000;
}

Area::~Area()
{
    m_blockPool.clear();
    m_currentArea.clear();
}

Area::Area()
	:m_startGame(false)
	,m_currentPoolIndex(0)
	,m_currentKitCounter(1)
	,m_currentKitIndex(0)
	,m_currentTypeIndex(0)
	,m_currentWidthIndex(0)
	,m_currentHeightIndex(0)
	,m_blockPool(20)
	,m_currentArea(20)
	,m_showEmpty(false)
	,m_blockData()
{
}

Area* Area::Create()
{
    auto terrain = new Area();
	if (terrain && terrain->initWithSpriteFrameName("blank.png"))
	{
        terrain->InitTerrain();
		terrain->autorelease();
		return terrain;
	}

	CC_SAFE_DELETE(terrain);
	return nullptr;
}

void Area::InitTerrain()
{
    m_screenSize = Director::getInstance()->getWinSize();
   
    m_emptyInterval = 0;
    m_currentEmptySize = 2;
    
	for (int i = 0; i < 20; i++)
	{
		auto block = Block::Create();
		addChild(block);
		m_blockPool.pushBack(block);
	}
	
	m_currentMinWidth = m_screenSize.width * 1.5f;
    
    random_shuffle(m_blockData.blockKits.begin(), m_blockData.blockKits.end());
    random_shuffle(m_blockData.blockWidths.begin(), m_blockData.blockWidths.end());
    random_shuffle(m_blockData.blockHeights.begin(), m_blockData.blockHeights.end());
	
	AddBlocks(0);
}

void Area::ActivateChimneys(Player* player)
{
    for (int i = 0; i < m_currentArea.size(); i++) 
	{
        auto block = m_currentArea.at(i);
		if (block->getType() == BlockType::Empty || block->getPuffing())
		{
			continue;
		}
                
		if (_position.x + block->getPositionX() >= m_screenSize.width * 0.2f &&
			_position.x + block->getPositionX() < m_screenSize.width * 0.8f)
		{
            block->SetPuffing(true);
        }
    }
}

void Area::CheckCollision(Player* player)
{
	if (player->getState() == PlayerState::PlayerDying)
	{
		return;
	}
    
	bool inAir = true;
	
    for (auto block : m_currentArea)
	{
        
		if (block->getType() == BlockType::Empty)
		{
			continue;
		}

		if (player->GetRight() >= getPositionX() + block->GetLeft() 
			&& player->GetLeft() <= getPositionX() + block->GetRight()
			&& player->GetBottom() >= block->GetTop()
			&& player->GetExpectedBottom() <= block->GetTop()
			&& player->GetTop() > block->GetTop())
		{
            player->setExpectedPosition(Vec2(player->getExpectedPosition().x, block->GetTop() + player->getHeight()));
			player->setVelocity(Vec2(player->getVelocity().x, 0));
            player->setRotation(0.0);
            inAir = false;
            break;
		}
	}
	
    for(auto block : m_currentArea) 
	{
		if (block->getType() == BlockType::Empty)
		{
			continue;
		}

		if (((player->GetBottom() < block->GetTop() && player->GetTop() > 0)
        || (player->GetExpectedBottom() < block->GetTop() && player->GetExpectedTop() > 0))
			&& player->GetRight() >= getPositionX() + block->getPositionX()
			&& player->GetLeft() < getPositionX() + block->getPositionX())
		{
            player->setPositionX(getPositionX() + block->getPositionX() - player->getWidth() * 0.5f);
            player->setExpectedPosition(Vec2(getPositionX() + block->getPositionX() - player->getWidth() * 0.5f, player->getExpectedPosition().y));
			player->setVelocity(Vec2(player->getVelocity().x * -0.5f, player->getVelocity().y));

			if (player->GetBottom() + player->getHeight() * 0.2f < block->GetTop())
			{
                player->setState(PlayerState::PlayerDying);
                SimpleAudioEngine::getInstance()->playEffect("hitBuilding.wav");
                return;
            }
            break;
		}
	}
    
	if (inAir)
	{
		player->setState(PlayerState::PlayerFalling);
	}
	else
	{
        if (player->getState() != PlayerState::PlayerMoving)
		{
            player->setState(PlayerState::PlayerMoving);
        }
		player->SetFloating(false);
	}
}
 

void Area::Move(float xMove) 
{
	if (xMove < 0)
	{
		return;
	}
    
    if (m_startGame)
	{
		if (xMove > 0 && m_currentEmptySize < 5)
		{
			m_emptyInterval += xMove; 
		}
        
        if (m_emptyInterval > c_increaseEmptyTimeInterval)
		{
            m_emptyInterval = 0;
            m_currentEmptySize++;
        }
    }
    
	setPositionX(_position.x - xMove);
    
    auto block =  m_currentArea.at(0);
	
	if (_position.x + block->getWidth() < 0) 
	{
        m_currentArea.erase(0);
        m_currentArea.pushBack(block);
        _position.x += block->getWidth();
		
		float width = GetAreaWidth() - block->getWidth() - (m_currentArea.at(0))->getWidth();
		InitBlock(block);
		AddBlocks(width);
	}
}

void Area::Reset()
{
    setPosition(Vec2(0,0));
    m_startGame = false;
    
    int currentWidth = 0;
    for (auto block : m_currentArea)
    {
        InitBlock(block);
		currentWidth += block->getWidth();
    }
    
	AddBlocks(currentWidth);
	SetBlocksPositions();

    m_emptyInterval = 0;
    m_currentEmptySize = 2;
}

std::vector<cocos2d::Point> Area::GetChimneysPos()
{
	std::vector<cocos2d::Point> chimneysPos;
	for (int i = 0; i < m_currentArea.size(); i++)
	{
		auto block = m_currentArea.at(i);
		if (block->getType() == BlockType::Empty)
		{
			continue;
		}

		if (_position.x + block->getPositionX() >= m_screenSize.width * 0.2f &&
			_position.x + block->getPositionX() < m_screenSize.width * 0.8f)
		{
			auto chimneyPos = block->getChimneys();
			for (auto const & chimney : chimneyPos)
			{
				auto pos = chimney->getPosition();
				if (pos != Vec2(0.f, 0.f))
				{
					auto it = std::find_if(chimneysPos.begin(), chimneysPos.end(), [&](const Vec2 position)
					{
						return position == pos;
					});
					if (it == chimneysPos.end())
					{
						chimneysPos.push_back(block->convertToWorldSpace(pos));
					}
				}
			}
		}
	}
	return chimneysPos;
}

void Area::AddBlocks(int currentWidth)
{
	while (currentWidth < m_currentMinWidth)
	{	
		auto block = m_blockPool.at(m_currentPoolIndex);
		m_currentPoolIndex++;
		m_currentPoolIndex = (m_currentPoolIndex == m_blockPool.size()) ? m_currentPoolIndex = 0 : m_currentPoolIndex;
		InitBlock(block);
		currentWidth += block->getWidth();
		m_currentArea.pushBack(block);
	}

	SetBlocksPositions();
}

void Area::SetBlocksPositions()
{
    Block * current;
	Block * prev;

	current = m_currentArea.at(0);
	current->setPositionX(0);

	for (ssize_t i = 1; i < m_currentArea.size(); i++)
	{
		current =  m_currentArea.at(i);
		prev = m_currentArea.at(i- 1);
		current->setPositionX(prev->getPositionX() + prev->getWidth());
	}
}

void Area::InitBlock(Block * block) 
{
    int blockWidth;
	int blockHeight;
    block->SetPuffing(false);
	
    BlockType type = static_cast<BlockType>(m_blockData.blockTypes[m_currentTypeIndex]);
	m_currentTypeIndex++;
	m_currentTypeIndex = (m_currentTypeIndex == m_blockData.blockTypes.size()) ? 0 : m_currentTypeIndex;

	if (m_startGame)
	{
        if (m_showEmpty)
		{
            int gap = rand() % m_currentEmptySize;
			gap = (gap < 2) ? 2 : gap;
            block->SetupBlock(gap, 0, BlockType::Empty);
			m_showEmpty = false;
        } 
		else
		{
			blockWidth = m_blockData.blockWidths[m_currentWidthIndex];
			
            if (m_blockData.blockHeights[m_currentHeightIndex] != 0)
			{
				blockHeight = m_blockData.blockHeights[m_currentHeightIndex];

				if (blockHeight - m_lastBlockHeight > 2 && m_currentEmptySize == 2)
				{
					blockHeight = 1;
				}
			} 
			else 
			{
                blockHeight = m_lastBlockHeight;
            }

			m_currentWidthIndex++;
			m_currentHeightIndex++;

			if (m_currentWidthIndex == m_blockData.blockWidths.size())
			{
				random_shuffle(m_blockData.blockWidths.begin(), m_blockData.blockWidths.end());
				m_currentWidthIndex = 0;
			}

            if (m_currentHeightIndex == m_blockData.blockHeights.size())
			{
                m_currentHeightIndex = 0;
                random_shuffle(m_blockData.blockHeights.begin(), m_blockData.blockHeights.end());
            }

			block->SetupBlock(blockWidth, blockHeight, type);
            m_lastBlockWidth = blockWidth;
            m_lastBlockHeight = blockHeight;
			
			m_currentKitCounter++;
			if (m_currentKitCounter >  m_blockData.blockKits[m_currentKitIndex])
			{
				m_showEmpty = true;
				m_currentKitIndex++;
				if (m_currentKitIndex == m_blockData.blockKits.size())
				{
                    random_shuffle(m_blockData.blockKits.begin(), m_blockData.blockKits.end());
					m_currentKitIndex = 0;
				}
				m_currentKitCounter = 1;
			}	
		}
	}
	else
	{
		m_lastBlockHeight = 2;
		m_lastBlockWidth = rand() % 2 + 2;
		block->SetupBlock(m_lastBlockWidth, m_lastBlockHeight, type);
	}
}

int Area::GetAreaWidth() const
{
	int width = 0;
	for (auto block : m_currentArea)
	{
		width += block->getWidth();
	}
	return width;
}

