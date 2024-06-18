#include "Game.h"
#include <SDL2/SDL_image.h>
#include <algorithm>
#include <iostream>
#include "Actor.h"
#include "Asteroid.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Random.h"

Game::Game()
    :mWindow(nullptr)
    ,mRenderer(nullptr)
    ,mIsRunning(true)
    ,mUpdatingActors(false)
    ,mRevivalTime(0.0f)
    ,mDeadTime(0.0f)
{

}

bool Game::Initialize()
{
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
    {
        SDL_Log("SDL の初期化に失敗しました: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 3)", 100, 100, 1024, 768, 0);
    if(!mWindow)
    {
        SDL_Log("ウインドウの生成に失敗しました: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!mRenderer)
    {
        SDL_Log("レンダラーの生成に失敗しました: %s", SDL_GetError());
        return false;
    }

    if(IMG_Init(IMG_INIT_PNG) == 0)
    {
        SDL_Log("SDL_image の初期化に失敗しました: %s", SDL_GetError());
        return false;
    }

    LoadData();

    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::RunLoop()
{
    while(mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor* actor)
{
    // アクター更新中なら待ちに追加
    if(mUpdatingActors)
    {
        mPendingActors.emplace_back(actor);
    } else 
    {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(Actor* actor)
{
	// 保留中のアクターの中にあるか確認する
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// 見つかった要素をベクターの末尾と交換し、末尾の要素を削除する（eraseによるコピーを避ける）
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// アクティブなアクターの中にあるか確認する
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// 見つかった要素をベクターの末尾と交換し、末尾の要素を削除する（eraseによるコピーを避ける）
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
    // 自分より順序が高い最初の位置に挿入 
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
	for ( ;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
    // イテレータの前に挿入
    mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	// erase を使用すると、削除された要素より後ろにある要素がコピーされるため、パフォーマンスに影響を与える可能性がありますが、スプライトの描画順序を維持するためにはこの方法が適しています。
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	// テクスチャはすでにマップに含まれているか
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// 画像をファイルからアップロード
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		// SDL_Surface を SDL_Textureに変更
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;   
}

void Game::AddAsteroid(Asteroid* ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);
	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
	}
}

void Game::ProcessInput()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                mIsRunning = false;
                break;
        }
    }

    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }

    mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keyState);
	}
	mUpdatingActors = false;
}

void Game::UpdateGame()
{
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if(deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }
    mTicksCount = SDL_GetTicks();

    mUpdatingActors = true;
    for(auto actor : mActors)
    {
        actor -> Update(deltaTime);
    }
    mUpdatingActors = false;

    // 復活用
    if(mShip->GetState() == Actor::EPaused)
    {
        mRevivalTime += deltaTime;
        //std::cout << mRevivalTime << std::endl;
        if(mRevivalTime >= 2.0f)
        {
            mShip->SetState(Actor::EActive);
	        mShip->SetPosition(Vector2(512.0f, 384.0f));
	        mShip->SetRotation(Math::PiOver2);

            mRevivalTime = 0.0f;
        }
    }

    for(auto pending : mPendingActors)
    {
        mActors.emplace_back(pending);
    }
    mPendingActors.clear();

    // 死んだアクターを一時配列に追加
    std::vector<Actor*> deadActors;

    for(auto actor : mActors)
    {
        if(actor->GetState() == Actor::EDead)
        {
            deadActors.emplace_back(actor);
        }
    }

    for(auto actor : deadActors)
    {
        delete actor;
    }
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	SDL_RenderClear(mRenderer);
	
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	SDL_RenderPresent(mRenderer);
}


void Game::LoadData()
{
    mShip = new Ship(this);
	mShip->SetPosition(Vector2(512.0f, 384.0f));
	mShip->SetRotation(Math::PiOver2);

    const int numAsteroids = 20;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
    // ~Actor では RemoveActor が呼び出されるので、別種のループを使う
    // .empty():空かどうかをチェック
    while(!mActors.empty())
    {
        delete mActors.back();
    }

    for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
}