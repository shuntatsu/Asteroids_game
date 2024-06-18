#pragma once
#include "Actor.h"
#include "CircleComponent.h"
#include "Asteroid.h"

class Ship : public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
	float mLaserCooldown;
    class CircleComponent* mCircle;

    float mRevivalTime;
    float mDeadTime;
};