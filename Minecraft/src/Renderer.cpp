#include "Renderer.h"

Renderer::Renderer()
{
	//m_Shader.Compile("#version 330 core\n"
	//	"layout (location = 0) in vec3 pos;\n"
	//	"layout (location = 1) in vec2 uv;\n"
	//	"out vec2 oUV;\n"
	//	"uniform mat4 pv;\n"
	//	"void main()\n"
	//	"{\n"
	//	"oUV = uv;\n"
	//	"gl_Position = pv * vec4(pos.xyz, 1.0);\n"
	//	"}\n",
	//
	//	"#version 330 core\n"
	//	"in vec2 oUV;\n"
	//	"out vec4 fragColor;\n"
	//	"uniform sampler2D uTexture;\n"
	//	"void main()\n"
	//	"{\n"
	//	"fragColor = texture(uTexture, vec2(oUV.x, 1.0 - oUV.y));\n"
	//	"}\n");
}

Renderer::~Renderer()
{
	//m_Shader.Destroy();
}

void Renderer::Prepare(const glm::mat4& pv)
{
	//Resources::GetTexture("block_atlas").Bind();
	//m_Shader.Use();
	//m_Shader.SetMat4("pv", pv);
}