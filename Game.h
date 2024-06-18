#pragma once
#include <SDL2\SDL.h>
#include <unordered_map>
#include <string>
#include <vector>

class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

    SDL_Texture* GetTexture(const std::string& fileName);

    // 惑星
    void AddAsteroid(class Asteroid* ast);
	void RemoveAsteroid(class Asteroid* ast);
	std::vector<class Asteroid*>& GetAsteroids() { return mAsteroids; }
    
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void LoadData();
    void UnloadData();

    // ロードされたテクスチャのマップ
    std::unordered_map<std::string, SDL_Texture*> mTextures;
    
    // ゲーム内のすべてのアクター
    std::vector<class Actor*> mActors;
    // 保留中のアクター
    std::vector<class Actor*> mPendingActors;
    
    // 描画されるすべてのスプライトコンポーネント
    std::vector<class SpriteComponent*> mSprites;

    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    Uint32 mTicksCount;

    bool mIsRunning;
    // 現在アクターを更新しているかを追跡
    bool mUpdatingActors;

    // プレイヤーの宇宙船
    class Ship* mShip;
    std::vector<class Asteroid*> mAsteroids;    
    float mRevivalTime;
    float mDeadTime;
};