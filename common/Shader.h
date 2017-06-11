#pragma once

#include <GL/glew.h>

#include <string>

class Shader {
public:
	Shader() = default;
	Shader(const std::string &vertex_shader_apth, const std::string& fragment_shader_path);

	void InitProgram(const std::string& vertex_code, const std::string& fragment_code);

	void Use();

	void SetInt(const std::string& name, int val);
	void SetFloat(const std::string& name, float val);

	int id() { return id_; }

private:
	GLuint id_;
};
