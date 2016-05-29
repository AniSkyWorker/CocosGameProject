#pragma once

#define PLAYER_INITIAL_SPEED 8
#define PLAYER_JUMP 42
#define G_FORCE 1.5
#define FLOATNG_GRAVITY 0.4
#define TERMINAL_VELOCITY 70
#define FLOATING_FRICTION 0.98f
#define AIR_FRICTION 0.99f

#include "cocos2d.h"
#include "GameSprite.h"

USING_NS_CC;

enum class PlayerState
{
    PlayerMoving,
    PlayerFalling,
    PlayerDying
}; 


class Player : public GameSprite
{
public:
	
	Player(void);
	virtual ~Player(void);
	
	CC_SYNTHESIZE(PlayerState, m_playerState, State);
	CC_SYNTHESIZE(bool, _inAir, InAir);
	CC_SYNTHESIZE_READONLY(bool, m_floating, Floating);
    CC_SYNTHESIZE(bool, m_jumping, Jumping);
    CC_SYNTHESIZE(float, m_maxSpeed, MaxSpeed);
    
    static Player * create (void);
	
	virtual void update (float dt);
    
    void setFloating (bool value);
    void reset (void);
    
	inline virtual void place () { 
		this->setPositionY( m_nextPosition.y );
        if (m_velocityVec.x > 0 && this->getPositionX() < m_screenSize.width * 0.2f) {
            this->setPositionX(this->getPositionX() + m_velocityVec.x);
            if (this->getPositionX() > m_screenSize.width * 0.2f) {
                this->setPositionX(m_screenSize.width * 0.2f);
            }
        }
	};
    
    inline int left() {
    	return this->getPositionX() - m_width * 0.5f;
	}
    
	inline int right() {
    	return this->getPositionX() + m_width * 0.5f;
	}
    
    inline int top() {
    	return this->getPositionY() ;
    }
    
    inline int bottom() {
		return this->getPositionY() - m_height  ;
    }
    
    inline int next_left() {
    	return m_nextPosition.x - m_width * 0.5f;
    }
    
    inline int next_right() {
    	return m_nextPosition.x + m_width * 0.5f;
    }
    
    inline int next_top() {
    	return m_nextPosition.y ;
    }
    
    inline int next_bottom() {
    	return m_nextPosition.y - m_height;
    }
private:
	Action * m_rideAnimation;
	Action * m_floatAnimation;
	float m_speed;
	int m_floatingTimerMax;
	float m_floatingTimer;
	int m_floatingInterval;
	bool m_hasFloated;

	Size m_screenSize;

	void initPlayer();
};
		
