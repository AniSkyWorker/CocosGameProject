#pragma once

#include "cocos2d.h"
#include "GameSprite.h"

enum class PlayerState
{
    PlayerMoving,
    PlayerFalling,
    PlayerDying
}; 

class Player : public GameSprite
{
public:
	
	Player();
	~Player() override;
	
	CC_SYNTHESIZE(PlayerState, m_gameState, State);
	CC_SYNTHESIZE(bool, _inAir, InAir);
	CC_SYNTHESIZE_READONLY(bool, m_floating, Floating);
    CC_SYNTHESIZE(bool, m_jumping, Jumping);
    CC_SYNTHESIZE(float, m_maxSpeed, MaxSpeed);
    
    static Player* Create();
	
	void update(float dt) override;
    
    void SetFloating(bool value);
    void Reset();
    
	void Move();
    
	int GetLeft() const;
	int GetRight() const;
	int GetTop() const;
	int GetBottom() const;

	int GetExpectedLeft() const;
	int GetExpectedRight() const;
    
	int GetExpectedTop() const;
	int GetExpectedBottom() const;

private:
	void InitPlayer();

	cocos2d::Action * m_rideAnimation;
	cocos2d::Action * m_floatAnimation;
	float m_speed;
	int m_floatingTimerMax;
	float m_floatingTimer;
	int m_floatingInterval;
	bool m_hasFloated;

	cocos2d::Size m_screenSize;
};
		
