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

#ifndef VBO_H
#define VBO_H

#include <GL/glew.h>

#include <vector>

#include "OpenGL.hpp"

namespace gl {
	enum BufferTarget : GLenum {
		ARRAY_BUFFER = GL_ARRAY_BUFFER,
		ATOMIC_COUNTER_BUFFER = GL_ATOMIC_COUNTER_BUFFER,
		COPY_READ_BUFFER = GL_COPY_READ_BUFFER,
		COPY_WRITE_BUFFER = GL_COPY_WRITE_BUFFER,
		DISPATCH_INDIRECT_BUFFER = GL_DISPATCH_INDIRECT_BUFFER,
		ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER,
		PIXEL_PACK_BUFFER = GL_PIXEL_PACK_BUFFER,
		PIXEL_UNPACK_BUFFER = GL_PIXEL_UNPACK_BUFFER,
		QUERY_BUFFER = GL_QUERY_BUFFER,
		SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
		TEXTURE_BUFFER = GL_TEXTURE_BUFFER,
		TRANSFORM_FEEDBACK_BUFFER = GL_TRANSFORM_FEEDBACK_BUFFER,
		UNIFORM_BUFFER = GL_UNIFORM_BUFFER,
		DRAW_INDIRECT_BUFFER = GL_DRAW_INDIRECT_BUFFER
	};
	
	enum BufferUsage {
		STATIC_DRAW = GL_STATIC_DRAW,
		DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		STREAM_DRAW = GL_STREAM_DRAW
	};

	class VBO {
	public:
		
		friend class VAO;
		
		VBO(uint32_t vertexSize, gl::BufferTarget target,
				gl::BufferUsage usage);
		~VBO();
		
		void Init();
		void Destroy();
		
		void Generate(const void* data, uint32_t vertexCount);
		void Generate(const std::vector<uint8_t>& data);
		
		void Fetch(void* data, uint32_t offset, uint32_t bytes);
		void FetchAll(std::vector<uint8_t>& data);
		void Update(const void* data, uint32_t offset, uint32_t bytes);
		
		void Resize(uint32_t newVertices);
		void Copy(VBO* sourceBuffer, uint32_t sourceOffset, uint32_t destinyOffset, uint32_t bytes);
		
		inline uint32_t VertexSize() const { return vertexSize; }
		
		inline uint32_t GetIdGL() const { return vboID; }
		
		inline uint32_t GetVertexCount() const { return vertices; }
		
		void BindBufferBase(gl::BufferTarget target, int location);
		
	private:
		
		gl::BufferTarget target;
		gl::BufferUsage usage;
		uint32_t vboID;
		uint32_t vertexSize, vertices;
	};
}

#endif
