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

#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

#include <vector>

#include "VBO.hpp"
#include "OpenGL.hpp"

namespace gl {
	enum VertexMode : GLenum {
		POINTS = GL_POINTS,
		LINE_STRIP = GL_LINE_STRIP,
		LINE_LOOP = GL_LINE_LOOP,
		LINES = GL_LINES,
		LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
		LINES_ADJACENCY = GL_LINES_ADJACENCY,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
		TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
		PATCHES = GL_PATCHES
	};

	class VAO {
	public:
		
		VAO(gl::VertexMode mode);
		~VAO();
		
		void Init();
		void Delete();
		
		// offset in bytes
		void SetAttribPointer(VBO& vbo, int location, unsigned count,
				gl::DataType type, bool normalized, unsigned offset,
				unsigned divisor=0);
		void BindElementBuffer(VBO& elementBO, gl::DataType type);
		void BindIndirectBuffer(VBO& indirectBO);
		
		void SetSize(unsigned count);
		void SetInstances(unsigned instances);
		
		void Bind();
		void Unbind();
		
		void Draw();
		void Draw(unsigned start, unsigned count);
		void DrawArrays(unsigned start, unsigned count);
		void DrawElements(unsigned start, unsigned count);
		void DrawMultiElementsIndirect(void* indirect, int drawCount);
		
	private:
	public:
		
		gl::DataType typeElements;
		unsigned sizeA, sizeI, instances;
		unsigned vaoID;
		gl::VertexMode mode;
		bool drawArrays;
		VBO* indirectDrawBuffer;
	};
}

#endif

