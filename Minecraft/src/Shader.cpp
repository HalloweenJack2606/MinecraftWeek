#include "Shader.h"


void Shader::Compile(const char* vs, const char* fs)
{
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex, 1, &vs, nullptr);
	glShaderSource(fragment, 1, &fs, nullptr);
	glCompileShader(vertex);
	glCompileShader(fragment);
	CheckErrors(vertex, "Vertex");
	CheckErrors(fragment, "Fragment");

	this->m_ID = glCreateProgram();
	glAttachShader(this->m_ID, vertex);
	glAttachShader(this->m_ID, fragment);
	glLinkProgram(this->m_ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::CheckErrors(const unsigned int& shader, const char* name)
{
	int status = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int loglen;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
		std::vector<char> log(loglen);
		GLsizei written;
		glGetShaderInfoLog(shader, loglen, &written, log.data());
		std::cout << name << " Compile Error: " << log.data() << std::endl;
	}
}

Shader& Shader::Use()
{
	glUseProgram(this->m_ID);
	return *this;
}

void Shader::Destroy()
{
	glDeleteProgram(this->m_ID);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
