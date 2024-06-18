#include "InputComponent.h"
#include "Actor.h"

InputComponent::InputComponent(class Actor* owner)
    :MoveComponent(owner)
    ,mForwardKey(0)
    ,mBackKey(0)
    ,mClockwiseKey(0)
    ,mCounterClockwiseKey(0)
{
	
}

/* 
関数: ProcessInput
    キーボードの状態に基づいて入力を処理する
引数:
    keyState - キーボードの状態を表す配列
戻り値:
    なし
*/
void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// MoveComponentの前進速度を計算
	float forwardSpeed = 0.0f;
	if (keyState[mForwardKey])
	{
		forwardSpeed += mMaxForwardSpeed;
	}
	if (keyState[mBackKey])
	{
		forwardSpeed -= mMaxForwardSpeed;
	}
	SetForwardSpeed(forwardSpeed);

	// MoveComponentの回転速度を計算
	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}
	SetAngularSpeed(angularSpeed);
}