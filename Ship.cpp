#include <iostream>
#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"

Ship::Ship(Game* game)
	:Actor(game)
	,mLaserCooldown(0.0f)
    ,mRevivalTime(0.0f)
    ,mDeadTime(0.0f)
{
	// スプライトコンポーネントを作成
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

    mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	// 入力コンポーネントを作成し、キーと速度を設定
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Ship::UpdateActor(float deltaTime)
{
	// レーザーのクールダウンを減少させる
	mLaserCooldown -= deltaTime;

	// 小惑星と衝突しているか確認
	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*mCircle, *(ast->GetCircle())))
		{
			// 自分自身を消滅させる
			SetState(EPaused);
			break;
		}
	}
}

void Ship::ActorInput(const uint8_t* keyState)
{
	// スペースキーが押されていて、レーザーのクールダウンが終了している場合
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// レーザーを作成し、自分の位置と回転を設定
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// レーザーのクールダウンをリセット（0.5秒）
		mLaserCooldown = 0.5f;
	}
}
