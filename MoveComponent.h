#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
    // updateOrder の値が小さいほど、先に更新
    MoveComponent(class Actor* owner, int updateOrder = 10);

    void Update(float deltaTime) override;

 	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }   
private:
    // 回転を制御（ラジアン / 秒）
    float mAngularSpeed;
    // 前進運動を制御（単位長 / 秒）
    float mForwardSpeed;
};