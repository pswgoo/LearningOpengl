#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

Shader::Shader(const std::string &vertex_shader_apth, const std::string& fragment_shader_path): id_(0) {
	ifstream vs_fin(vertex_shader_apth);
	ifstream fs_fin(fragment_shader_path);

	stringstream vs_sin;
	vs_sin << vs_fin.rdbuf();

	stringstream fs_sin;
	fs_sin << fs_fin.rdbuf();

	InitProgram(vs_sin.str(), fs_sin.str());
}

static char g_info_buffer[1024];

void Shader::InitProgram(const std::string& vertex_code, const std::string& fragment_code) {
	GLuint vs_shader = glCreateShader(GL_VERTEX_SHADER);
	const char* vs_code = vertex_code.data();
	glShaderSource(vs_shader, 1, &vs_code, nullptr);
	glCompileShader(vs_shader);

	GLint success;
	glGetShaderiv(vs_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vs_shader, sizeof(g_info_buffer), nullptr, g_info_buffer);
		cout << "Vertex Shader Error: " << g_info_buffer << std::endl;
	}

	GLuint fs_shader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fs_code = fragment_code.data();
	glShaderSource(fs_shader, 1, &fs_code, nullptr);
	glCompileShader(fs_shader);

	glGetShaderiv(fs_shader, GL_COMPILE, &success);
	if (!success) {
		glGetShaderInfoLog(fs_shader, sizeof(g_info_buffer), nullptr, g_info_buffer);
		cout << "Fragment Shader Error: " << g_info_buffer << std::endl;
	}
	
	id_ = glCreateProgram();
	glAttachShader(id_, vs_shader);
	glAttachShader(id_, fs_shader);

	glLinkProgram(id_);

	glGetProgramiv(id_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id_, sizeof(g_info_buffer), nullptr, g_info_buffer);
		cout << "Shader Program Error: " << g_info_buffer << std::endl;
	}

	glDeleteShader(vs_shader);
	glDeleteShader(fs_shader);
}

void Shader::Use() {
	glUseProgram(id_);
}

void Shader::SetInt(const std::string& name, int val) {
	glUniform1i(glGetUniformLocation(id_, name.data()), (GLint)val);
}

void Shader::SetFloat(const std::string& name, float val) {
	glUniform1f(glGetUniformLocation(id_, name.data()), val);
}

