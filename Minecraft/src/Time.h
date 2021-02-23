#pragma once
#include <cstdint>

class Application;
class Time
{
public:
	static float deltaTime;
	static float time;
	static std::uint32_t frameCount;
private:
	friend class Application;
	inline static void SetTime(const float& dt) { deltaTime = dt; time += dt; }
};