#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
	,mTexture(nullptr)
	,mDrawOrder(drawOrder)
	,mTexWidth(0)
	,mTexHeight(0)
{
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect r;
		// オーナーのスケールに基づいて描画領域のサイズを調整
		r.w = static_cast<int>(mTexWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTexHeight * mOwner->GetScale());
		// 描画領域をオーナーの位置に中心合わせ
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

        // 描画 (角度をラジアンから度に変換し、時計回りから反時計回りに変換)
        /*
            renderer: 描画するためのレンダラー。
            texture: 描画するテクスチャ。
            src: ソース矩形（省略可能）。
            dst: 宛先矩形。
            angle: 回転角度（度単位）。
            center: 回転の中心座標（省略可能）。
            flip: フリッピングの方向（水平、垂直、両方）。
         */
		SDL_RenderCopyEx(
            renderer,
			mTexture,
			nullptr,
			&r,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr,
			SDL_FLIP_NONE);
	}
}

// mTexWidthとmTexHeightにテクスチャーの幅と高さが格納される
void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;
	// Query:問い合わせ 
	SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}
