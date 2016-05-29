#include "Player.h"
#include "SimpleAudioEngine.h"

namespace
{
	const double c_acceleraton = 0.05;
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
    m_speed = PLAYER_INITIAL_SPEED;
    m_maxSpeed = PLAYER_INITIAL_SPEED;
    m_floating = false;
    m_nextPosition = Vec2(0,0);
	m_nextPosition.y = m_screenSize.height * 0.6f;
	m_playerState = PlayerState::PlayerMoving;
    m_jumping = false;
    m_hasFloated = false;
}

Player * Player::create()
{
    auto player = new Player();
    
    if (player && player->initWithSpriteFrameName("player_1.png"))
	{
		player->autorelease();
        player->setSize();
        player->initPlayer();
        return player;
	}
    
	CC_SAFE_DELETE(player);
	return nullptr;
}


void Player::update(float dt)
{
	m_speed = m_speed + c_acceleraton <= m_maxSpeed ? m_speed + c_acceleraton : m_speed;
    m_velocityVec.x = m_speed;
    
	switch (m_playerState)
	{
	case PlayerState::PlayerMoving:
		m_velocityVec.y -= G_FORCE;
		if (m_hasFloated)
		{
			m_hasFloated = false;
		}
		break;

	case PlayerState::PlayerFalling:
		if (m_floating)
		{
			m_velocityVec.y -= FLOATNG_GRAVITY;
			m_velocityVec.x *= FLOATING_FRICTION;
			
		}
		else
		{
			m_velocityVec.y -= G_FORCE;
			m_velocityVec.x *= AIR_FRICTION;
			m_floatingTimer = 0;
		}
		break;
	case PlayerState::PlayerDying:
        m_velocityVec.y -= G_FORCE;
        m_velocityVec.x = -m_speed;
        setPositionX(getPositionX() + m_velocityVec.x);
        break;
	}
    
    if (m_jumping)
	{
        m_playerState = PlayerState::PlayerFalling;
        m_velocityVec.y += PLAYER_JUMP * 0.25f;
		if (m_velocityVec.y > PLAYER_JUMP)
		{
			m_jumping = false;
		}
    }
    
	if (m_velocityVec.y < -TERMINAL_VELOCITY)
	{
		m_velocityVec.y = -TERMINAL_VELOCITY;
	}
    
  	m_nextPosition.y = getPositionY() + m_velocityVec.y;
    
        
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
			setFloating(false);
		}
	}
}

void Player::reset() 
{
    m_speed = PLAYER_INITIAL_SPEED;
    m_maxSpeed = PLAYER_INITIAL_SPEED;
    
    m_velocityVec = Vec2(0,0);
    this->setFloating(false);
    this->setRotation(0);
    m_nextPosition.y = m_screenSize.height * 0.6f;
    this->setPosition(Vec2( m_screenSize.width * 0.2f, m_nextPosition.y ));
    m_playerState = PlayerState::PlayerMoving;
    m_jumping = false;
    m_hasFloated = false;
}

void Player::setFloating(bool value)
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
        setDisplayFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("player_float.png"));
        runAction(m_floatAnimation);
        m_velocityVec.y += PLAYER_JUMP * 0.5f;
    } 
	else
	{
        runAction(m_rideAnimation);
    }
}

void Player::initPlayer()
{
    setAnchorPoint(Vec2(0.5f, 1.0f));
    setPosition(Vec2(m_screenSize.width * 0.2f, m_nextPosition.y));
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
