#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

class Actor
{
public:
    // アクターの状態管理用 (e は、"enumeration"（列挙型）)
    enum State
    {
        EActive,
        EPaused,
        EDead
    };

    // コンストラクター、デストラクター
    Actor(class Game* game);
    virtual ~Actor();

    // Game から呼び出される更新関数(オーバーライト不可)
    void Update(float deltaTime);
    // アクターが持つ全コンポーネントを更新(オーバーライド不可)
    void UpdateComponents(float deltatime);
    // アクター独自の更新処理(オーバーライト可能)
    virtual void UpdateActor(float deltaTime);

    // Game から呼び出される ProcessInput 関数（オーバーライド不可）
	void ProcessInput(const uint8_t* keyState);
	// アクター固有の入力コード（オーバーライド可能）
	virtual void ActorInput(const uint8_t* keyState);

    // ゲッター / セッター
    const Vector2& GetPosition() const { return mPosition; }
    void SetPosition(const Vector2& pos) { mPosition = pos; }
    float GetScale() const { return mScale; }
    void SetScale(float scale) { mScale = scale; } 
    float GetRotation() const { return mRotation; }
    void SetRotation(float rotation) { mRotation = rotation; }

    // 前方ベクトル
    Vector2 GetForward() const 
    { 
        return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); 
    }

    State GetState() const { return mState; }
    void SetState(State state) { mState = state; }

    class Game* GetGame() { return mGame; }

    // コンポーネントの追加 / 削除
    void AddComponent(class Component* conmponet);
    void RemoveComponent(class Component* component);

private:
    // アクターの状態
    State mState;

    // 座標変換
    Vector2 mPosition;  // アクターの中心位置
    float mScale;       // アクターのスケール (1.0f が 100%)
    float mRotation;    // 回転の角度 (ラジアン)

    // アクターが持つコンポーネント
    std::vector<class Component*> mComponents;
    class Game* mGame;
};