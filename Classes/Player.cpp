#include "Player.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

namespace
{
	const float c_acceleraton = 0.05f;
	const float c_velocity = 8.f;
	const float c_jump = 40.f;
	const float c_gravityForce = 1.5f;
	const float c_floatingForce = 0.4f;
	const float с_terminalVelocity = 70.f;
	const float c_floatingFriction = 0.96f;
	const float c_airFriction = 0.99f;
}
using namespace CocosDenshion;

Player::~Player()
{
    CC_SAFE_RELEASE(m_rideAnimation);
    CC_SAFE_RELEASE(m_floatAnimation);
}

Player::Player()
{
    m_screenSize = Director::getInstance()->getWinSize();
	m_floatingTimerMax = 2;
	m_floatingTimer = 0;
	Reset();
}

Player * Player::Create()
{
    auto player = new Player();
    
    if (player && player->initWithSpriteFrameName("player_1.png"))
	{
		player->autorelease();
		player->setWidth(player->getBoundingBox().size.width);
		player->setHeight(player->getBoundingBox().size.height);
        player->InitPlayer();
        return player;
	}
    
	CC_SAFE_DELETE(player);
	return nullptr;
}


void Player::update(float dt)
{
	m_speed = m_speed + c_acceleraton <= m_maxSpeed ? m_speed + c_acceleraton : m_speed;
    m_velocityVec.x = m_speed;
    
	switch (m_gameState)
	{
	case PlayerState::PlayerMoving:
		m_velocityVec.y -= c_gravityForce;
		if (m_hasFloated)
		{
			m_hasFloated = false;
		}
		break;

	case PlayerState::PlayerFalling:
		if (m_floating)
		{
			m_velocityVec.y -= c_floatingForce;
			m_velocityVec.x *= c_floatingFriction;
		}
		else
		{
			m_velocityVec.y -= c_gravityForce;
			m_velocityVec.x *= c_airFriction;
			m_floatingTimer = 0;
		}
		break;
	case PlayerState::PlayerDying:
        m_velocityVec.y -= c_gravityForce;
        m_velocityVec.x = -m_speed;
        setPositionX(getPositionX() + m_velocityVec.x);
        break;
	}
    
    if (m_jumping)
	{
        m_gameState = PlayerState::PlayerFalling;
        m_velocityVec.y += c_jump * 0.25f;
		if (m_velocityVec.y > c_jump)
		{
			m_jumping = false;
		}
    }
    
	if (m_velocityVec.y < -с_terminalVelocity)
	{
		m_velocityVec.y = -с_terminalVelocity;
	}
    
  	m_expectedPosition.y = getPositionY() + m_velocityVec.y;
    
        
	if (pow(m_velocityVec.x, 2) < 0.01)
	{
		m_velocityVec.x = 0;
	}
	if (pow(m_velocityVec.y, 2) < 0.01)
	{
		m_velocityVec.y = 0;
	}
	
	if (m_floating)
	{
		m_floatingTimer += dt;
		if (m_floatingTimer > m_floatingTimerMax)
		{
            m_floatingTimer = 0;
            SimpleAudioEngine::getInstance()->playEffect("falling.wav");
			SetFloating(false);
		}
	}
}

void Player::Reset() 
{
    m_speed = c_velocity;
    m_maxSpeed = c_velocity;
    m_velocityVec = Vec2(0,0);

	SetFloating(false);
    setRotation(0);

    m_expectedPosition.y = m_screenSize.height * 0.6f;
    setPosition(Vec2( m_screenSize.width * 0.2f, m_expectedPosition.y));

    m_gameState = PlayerState::PlayerMoving;

    m_jumping = false;
    m_hasFloated = false;
}

void Player::Move()
{
	setPositionY(m_expectedPosition.y);
	if (m_velocityVec.x > 0 && _position.x < m_screenSize.width * 0.2f)
	{
		setPositionX(_position.x + m_velocityVec.x);
		if (_position.x > m_screenSize.width * 0.2f)
		{
			setPositionX(m_screenSize.width * 0.2f);
		}
	}
}

int Player::GetLeft() const
{
	return _position.x - m_width * 0.5f;
}

int Player::GetRight() const
{
	return _position.x + m_width * 0.5f;
}

int Player::GetTop() const
{
	return _position.y;
}

int Player::GetBottom() const
{
	return _position.y - m_height;
}

int Player::GetExpectedLeft() const
{
	return m_expectedPosition.x - m_width * 0.5f;
}

int Player::GetExpectedRight() const
{
	return m_expectedPosition.x + m_width * 0.5f;
}

int Player::GetExpectedTop() const
{
	return m_expectedPosition.y;
}

int Player::GetExpectedBottom() const
{
	return m_expectedPosition.y - m_height;
}

void Player::SetFloating(bool value)
{
	if (m_floating == value || (value && m_hasFloated))
	{
		return;
	}
    
    m_floating = value;
    
    stopAllActions();
    
    if (value)
	{
        m_hasFloated = true;
        SimpleAudioEngine::getInstance()->playEffect("openUmbrella.wav");
        setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("player_float.png"));
        runAction(m_floatAnimation);
        m_velocityVec.y += c_jump * 0.5f;
    } 
	else
	{
        runAction(m_rideAnimation);
    }
}

void Player::InitPlayer()
{
    setAnchorPoint(Vec2(0.5f, 1.0f));
    setPosition(Vec2(m_screenSize.width * 0.2f, m_expectedPosition.y));
    m_height = 252 * 0.95f;
    m_width = 184;
    
    Animation* animation;
    animation = Animation::create();
    SpriteFrame * frame;
    for(int i = 1; i <= 3; i++) 
	{
        frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("player_%i.png", i)->getCString());
        animation->addSpriteFrame(frame);
    }
    
    animation->setDelayPerUnit(0.2f / 3.0f);
    animation->setRestoreOriginalFrame(false);
    animation->setLoops(-1);
    m_rideAnimation = Animate::create(animation);
    m_rideAnimation->retain();
    
    
    auto easeSwing = Sequence::create(
           EaseInOut::create(RotateTo::create(0.8f, -10), 2),
           EaseInOut::create(RotateTo::create(0.8f, 10), 2),
           nullptr);
    m_floatAnimation = RepeatForever::create( (ActionInterval*) easeSwing );
    m_floatAnimation->retain();
    
    this->runAction(m_rideAnimation);
}
