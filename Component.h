#pragma once
#include <cstdint>
class Component
{
public:
    // コンストラクター
    // updateOrder が小さいコンポーネントほど早く更新される
    Component(class Actor* owner, int updateOrder = 100);
    // デストラクター
    virtual ~Component();
    // このコンポーネントをデルタタイムで更新する
    virtual void Update(float deltaTime);
    int GetUpdateOrder() const { return mUpdateOrder; }

    virtual void ProcessInput(const uint8_t* keyState) {}

// protected:クラス自身とその派生クラスからアクセス可能
protected:
    // 所有アクター
    class Actor* mOwner;
    // コンポーネントの更新順序
    int mUpdateOrder;
};