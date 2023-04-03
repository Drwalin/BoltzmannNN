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

#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace gl {
	enum ShaderType : GLenum {
		VERTEX_SHADER = GL_VERTEX_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		COMPUTE_SHADER = GL_COMPUTE_SHADER
	};

	class Shader {
	public:
		
		int Compile(const std::string& vertexCode, const std::string& geometryCode,
				const std::string& fragmentCode);		// return 0 if no errors
		int Compile(const std::string& computeCode);	// return 0 if no errors
		
		int Load(const std::string& vertexPath, const std::string& geometryPath,
				const std::string& fragmentPath);		// return 0 if no errors
		int Load(const std::string& computePath);		// return 0 if no error

		void Use();
		unsigned GetProgram();
		void Dispatch(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);
		void DispatchRoundGroupNumbers(uint32_t numGroupsX, uint32_t numGroupsY, uint32_t numGroupsZ);
		
		int GetUniformLocation(const char* name) const;
		int GetAttributeLocation(const char* name) const;
		int GetUniformLocation(const std::string& name) const;
		int GetAttributeLocation(const std::string& name) const;
		
		void SetTexture(int location, class Texture* texture, unsigned textureId);
		void SetBool(int location, bool value);
		void SetUInt(int location, uint32_t value);
		void SetInt(int location, int value);
		void SetInt(int location, const std::vector<int>& array);
		void SetUInt(int location, const std::vector<uint32_t>& array);
		void SetFloat(int location, float value);
		void SetFloat(int location, const std::vector<float>& array);
		void SetVec2(int location, const glm::vec2 &value);
		void SetVec3(int location, const glm::vec3 &value);
		void SetVec4(int location, const glm::vec4 &value);
		void SetVec2(int location, const std::vector<glm::vec2>& array);
		void SetVec3(int location, const std::vector<glm::vec3>& array);
		void SetVec4(int location, const std::vector<glm::vec4>& array);
		void SetMat2(int location, const glm::mat2 &mat);
		void SetMat3(int location, const glm::mat3 &mat);
		void SetMat4(int location, const glm::mat4 &mat);
		void SetMat4(int location, const std::vector<glm::mat4>& array);
		
		void Destroy();
		
		Shader();
		~Shader();
		
		int32_t workgroupSize[3];
		
	private:
		
		std::string LoadFile(const std::string& filePath);
		
		unsigned CheckBuildStatus();
		
		static unsigned currentProgram;
		
		static unsigned CompileGLSL(const std::string& code, gl::ShaderType type);
		static void PrintCode(const std::string& code);
		
		unsigned int program;
	};
}

#endif

