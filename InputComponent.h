#pragma once
#include "MoveComponent.h"
#include <cstdint>

class InputComponent : public MoveComponent
{
public:
	// 更新順序を下げて最初に更新する
	InputComponent(class Actor* owner);

	void ProcessInput(const uint8_t* keyState) override;
	
	// プライベート変数のゲッター/セッター
	float GetMaxForward() const { return mMaxForwardSpeed; }
	float GetMaxAngular() const { return mMaxAngularSpeed; }
	int GetForwardKey() const { return mForwardKey; }
	int GetBackKey() const { return mBackKey; }
	int GetClockwiseKey() const { return mClockwiseKey; }
	int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

	void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
	void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
	void SetForwardKey(int key) { mForwardKey = key; }
	void SetBackKey(int key) { mBackKey = key; }
	void SetClockwiseKey(int key) { mClockwiseKey = key; }
	void SetCounterClockwiseKey(int key) { mCounterClockwiseKey = key; }
private:
	// 最大前進/回転速度
	float mMaxForwardSpeed;
	float mMaxAngularSpeed;
	// 前進/後退のためのキー
	int mForwardKey;
	int mBackKey;
	// 回転のためのキー
	int mClockwiseKey;
	int mCounterClockwiseKey;
};
