#include "Frustum.h"

glm::vec4* Frustum::s_Planes = new glm::vec4[6];

bool Frustum::PlaneIntersection(const glm::mat4& pv, const glm::vec3& position, const float skin)
{
	for (int i = 0; i < 6; i++)
	{
		auto p = glm::normalize(s_Planes[i]);
		glm::vec3 normal = glm::vec3(p.x, p.y, p.z);
		float distance = p.w;
		if (glm::dot(position, normal) + distance + skin <= 0) return false;
	}
	return true;
}

void Frustum::ExtractPlanes(const glm::mat4& pv)
{
	//@note: Gribb/Hartmann method
	auto mat = pv;
	for (int i = 4; i--;) s_Planes[0][i] = mat[i][3] + mat[i][0];
	for (int i = 4; i--;) s_Planes[1][i] = mat[i][3] - mat[i][0];
	for (int i = 4; i--;) s_Planes[3][i] = mat[i][3] + mat[i][1];
	for (int i = 4; i--;) s_Planes[2][i] = mat[i][3] - mat[i][1];
	for (int i = 4; i--;) s_Planes[4][i] = mat[i][3] + mat[i][2];
	for (int i = 4; i--;) s_Planes[5][i] = mat[i][3] - mat[i][2];
}