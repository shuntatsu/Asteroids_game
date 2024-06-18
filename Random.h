#pragma  once
#include <random>
#include "Math.h"

class Random
{
public:
	static void Init();

	// 指定された整数で乱数生成器をシードする
	static void Seed(unsigned int seed);

	// 0.0fから1.0fの間の浮動小数点数を取得する
	static float GetFloat();
	
	// 指定された範囲内の浮動小数点数を取得する
	static float GetFloatRange(float min, float max);

	// 指定された範囲内の整数を取得する
	static int GetIntRange(int min, int max);

	// 指定された範囲内のランダムなベクトルを取得する
	static Vector2 GetVector(const Vector2& min, const Vector2& max);
	static Vector3 GetVector(const Vector3& min, const Vector3& max);
private:
	static std::mt19937 sGenerator;
};