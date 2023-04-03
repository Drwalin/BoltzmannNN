/*
 *  This file is part of OpenGLWrapper.
 *  Copyright (C) 2021-2023 Marek Zalewski aka Drwalin
 *
 *  OpenGLWrapper is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  OpenGLWrapper is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <cstdio>

#include "../include/openglwrapper/Texture.hpp"

#include "../include/openglwrapper/Shader.hpp"

namespace gl {

unsigned Shader::currentProgram = 0;

int Shader::Compile(const std::string& vertexCode, const std::string& geometryCode,
		const std::string& fragmentCode) {
	Destroy();
	
	unsigned vertex   = Shader::CompileGLSL(vertexCode, gl::VERTEX_SHADER);
	unsigned geometry = Shader::CompileGLSL(geometryCode, gl::GEOMETRY_SHADER);
	unsigned fragment = Shader::CompileGLSL(fragmentCode, gl::FRAGMENT_SHADER);
	
	GL_CHECK_PUSH_ERROR;
	
	program = glCreateProgram();
	if(geometry)
		glAttachShader(program, geometry);
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
	
	GL_CHECK_PUSH_ERROR;
	
	glLinkProgram(program);
	
	GL_CHECK_PUSH_ERROR;
	
	if(vertex)
		glDeleteShader(vertex);
	if(geometry)
		glDeleteShader(geometry);
	if(fragment)
		glDeleteShader(fragment);
	
	GL_CHECK_PUSH_ERROR;
	
	return CheckBuildStatus();
}

int Shader::Compile(const std::string& computeCode) {
	Destroy();
	
	unsigned compute = Shader::CompileGLSL(computeCode, gl::COMPUTE_SHADER);
	
	program = glCreateProgram();
	glAttachShader(program, compute);
	glLinkProgram(program);
	
	if(compute)
		glDeleteShader(compute);
	
	int ret = CheckBuildStatus();
	if(ret == 0) {
		glGetProgramiv(program, GL_COMPUTE_WORK_GROUP_SIZE, workgroupSize);
	}
	GL_CHECK_PUSH_ERROR;
	return CheckBuildStatus();
}

int Shader::Load(const std::string& vertexPath, const std::string& geometryPath,
		const std::string& fragmentPath) {
	return Compile(LoadFile(vertexPath), LoadFile(geometryPath), LoadFile(fragmentPath));
}

int Shader::Load(const std::string& computePath) {
	return Compile(LoadFile(computePath));
}

void Shader::Dispatch(uint32_t numGroupsX, uint32_t numGroupsY,
		uint32_t numGroupsZ) {
	if(numGroupsX==0 || numGroupsY==0 || numGroupsZ==0)
		return;
	glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
	GL_CHECK_PUSH_ERROR;
}

void Shader::DispatchRoundGroupNumbers(uint32_t numGroupsX, uint32_t numGroupsY,
		uint32_t numGroupsZ) {
	if(numGroupsX==0 || numGroupsY==0 || numGroupsZ==0)
		return;
	glDispatchCompute(
			this->workgroupSize[0]*((numGroupsX-1+this->workgroupSize[0])/this->workgroupSize[0]),
			this->workgroupSize[1]*((numGroupsY-1+this->workgroupSize[1])/this->workgroupSize[1]),
			this->workgroupSize[2]*((numGroupsZ-1+this->workgroupSize[2])/this->workgroupSize[2])
			);
	GL_CHECK_PUSH_ERROR;
}

unsigned Shader::CheckBuildStatus() {
	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
		GL_CHECK_PUSH_ERROR;
	if(!success) {
		char infoLog[512];
		int size;
		glGetProgramInfoLog(program, 512, &size, infoLog);
		printf("\n ERROR::SHADER::PROGRAM::LINKING_FAILED\n %s", infoLog);
		GL_CHECK_PUSH_ERROR;
		return 3;
	}
	return 0;
}

unsigned Shader::CompileGLSL(const std::string& code, gl::ShaderType type) {
	char const* shaderStrType = 0;
	switch(type) {
		case VERTEX_SHADER:
			shaderStrType = "VERTEX";
			break;
		case FRAGMENT_SHADER:
			shaderStrType = "FRAGMENT";
			break;
		case GEOMETRY_SHADER:
			shaderStrType = "GEOMETRY";
			break;
		case COMPUTE_SHADER:
			shaderStrType = "COMPUTE";
			break;
		default:
			throw "Unknown shader type in Shader::CompileGLSL";
			return 0;
	}
	
	GL_CHECK_PUSH_ERROR;
	if(code != "") {
		int success;
		char infoLog[5120];
		unsigned program = glCreateShader(type);
	GL_CHECK_PUSH_ERROR;
		const char* pcode = code.c_str();
		glShaderSource(program, 1, &pcode, NULL);
	GL_CHECK_PUSH_ERROR;
		glCompileShader(program);
	GL_CHECK_PUSH_ERROR;
		glGetShaderiv(program, GL_COMPILE_STATUS, &success);
	GL_CHECK_PUSH_ERROR;
		if(!success) {
			GLsizei size;
			glGetShaderInfoLog(program, 5120, &size, infoLog);
	GL_CHECK_PUSH_ERROR;
			printf("\n ERROR::SHADER::%s::COMPILATION_FAILED\n %s",
					shaderStrType, infoLog);
			PrintCode(code);
			glDeleteShader(program);
	GL_CHECK_PUSH_ERROR;
			return 0;
		}
		return program;
	}
	return 0;
}

std::string Shader::LoadFile(const std::string& filePath) {
	std::string code;
	std::ifstream file(filePath, std::ios::binary|std::ios::in);
	if(!file.good())
		return "";
	
	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::end);
	fileSize = (size_t)file.tellg() - fileSize;
	file.seekg(0, std::ios::beg);
	
	code.resize(fileSize+1);
	file.read(code.data(), fileSize);
	code[fileSize] = 0;
	
	file.close();
	return code;
}

void Shader::PrintCode(const std::string& code) {
	printf("\n Code: \n    1: ");
	int line = 1;
	int x=0;
	for(char const* i = code.c_str(); *i; ++i) {
		switch(*i) {
			case '\n':
				x = 0;
				++line;
				printf("\n%5i: ", line);
				break;
			case '\r':
				break;
			case '\t':
				for(int t=((x+4)>>2)<<2; x < t; ++x)
					putchar(' ');
				break;
			case 0:
				return;
			default:
				putchar(*i);
				++x;
		}
	}
}

void Shader::Use() {
	if(program) {
		if(currentProgram != program)
			glUseProgram(program);
		GL_CHECK_PUSH_ERROR;
		currentProgram = program;
	}
}

unsigned int Shader::GetProgram() {
	return program;
}

int Shader::GetUniformLocation(const char * name) const {
	return glGetUniformLocation(program, name);
}

int Shader::GetAttributeLocation(const char * name) const {
	int v = glGetAttribLocation(program, name);
	return v;
}

int Shader::GetUniformLocation(const std::string& name) const {
	return GetUniformLocation(name.c_str());
}

int Shader::GetAttributeLocation(const std::string& name) const {
	return GetAttributeLocation(name.c_str());
}

void Shader::SetTexture(int location, class Texture* texture,
		unsigned textureId) {
	glActiveTexture(GL_TEXTURE0+textureId);
		GL_CHECK_PUSH_ERROR;
	if(texture) {
		texture->Bind();
		GL_CHECK_PUSH_ERROR;
		SetInt(location, textureId);
		GL_CHECK_PUSH_ERROR;
	} else {
		glBindTexture(GL_TEXTURE_2D, 0);
		GL_CHECK_PUSH_ERROR;
	}
	SetInt(location, textureId);
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetBool(int location, bool value) {
	Use();
	glUniform1i(location, (int)value); 
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetUInt(int location, uint32_t value) {
		GL_CHECK_PUSH_ERROR;
	Use();
		GL_CHECK_PUSH_ERROR;
	glProgramUniform1ui(program, location, value);
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetInt(int location, int value) {
		GL_CHECK_PUSH_ERROR;
	Use();
		GL_CHECK_PUSH_ERROR;
	glProgramUniform1i(program, location, value);
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetInt(int location, const std::vector<int>& array) {
	Use();
	glUniform1uiv(location, array.size(), (unsigned int*)&array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetUInt(int location, const std::vector<unsigned int>& array) {
	Use();
	glUniform1uiv(location, array.size(), &array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetFloat(int location, float value) { 
	Use();
	glUniform1f(location, value); 
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetFloat(int location, const std::vector<float>& array) {
	Use();
	glUniform1fv(location, array.size(), &array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetVec2(int location, const glm::vec2 &value) {
	Use();
	glUniform2fv(location, 1, glm::value_ptr(value));
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetVec3(int location, const glm::vec3 &value) {
	Use();
	glUniform3fv(location, 1, glm::value_ptr(value));
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetVec2(int location, const std::vector<glm::vec2>& array) {
	Use();
	glUniform2fv(location, array.size(), (float*)&array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetVec3(int location, const std::vector<glm::vec3>& array) {
	Use();
	glUniform3fv(location, array.size(), (float*)&array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetVec4(int location, const std::vector<glm::vec4>& array) {
	Use();
	glUniform4fv(location, array.size(), (float*)&array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetVec4(int location, const glm::vec4 &value) {
	Use();
	glUniform4fv(location, 1, glm::value_ptr(value));
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetMat2(int location, const glm::mat2 &mat) {
	Use();
	glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetMat3(int location, const glm::mat3 &mat) {
	Use();
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetMat4(int location, const glm::mat4 &mat) {
	Use();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
		GL_CHECK_PUSH_ERROR;
}

void Shader::SetMat4(int location, const std::vector<glm::mat4>& array) {
	Use();
	glUniformMatrix4fv(location, array.size()*16*4, GL_FALSE,
			(float*)&array.front());
		GL_CHECK_PUSH_ERROR;
}

void Shader::Destroy() {
	if(program) {
	GL_CHECK_PUSH_ERROR;
		currentProgram = 0;
		glUseProgram(0);
		glDeleteProgram(program);
		program = 0;
	GL_CHECK_PUSH_ERROR;
	}
}

Shader::Shader() {
	program = 0;
}

Shader::~Shader() {
	Destroy();
}

}

