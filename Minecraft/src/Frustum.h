#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Frustum
{
private:
	static glm::vec4* s_Planes;
public:
	static bool PlaneIntersection(const glm::mat4& pv, const glm::vec3& position, const float skin);
	static void ExtractPlanes(const glm::mat4& pv);
};