/*
 *  This file is part of OpenGLWrapper.
 *  Copyright (C) 2023 Marek Zalewski aka Drwalin
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

#ifndef FBO_H
#define FBO_H

#include <GL/glew.h>

#include <vector>

#include "Texture.hpp"
#include "OpenGL.hpp"

namespace gl {
	enum FboAttachmentType : GLenum {
		ATTACHMENT_DEPTH = GL_DEPTH_ATTACHMENT,
		ATTACHMENT_STENCIL = GL_STENCIL_ATTACHMENT,
		ATTACHMENT_DEPTH_STENCIL = GL_DEPTH_STENCIL_ATTACHMENT,
		ATTACHMENT_COLOR0 = GL_COLOR_ATTACHMENT0,
		ATTACHMENT_COLOR1 = GL_COLOR_ATTACHMENT1,
		ATTACHMENT_COLOR2 = GL_COLOR_ATTACHMENT2,
		ATTACHMENT_COLOR3 = GL_COLOR_ATTACHMENT3,
		ATTACHMENT_COLOR4 = GL_COLOR_ATTACHMENT4,
		ATTACHMENT_COLOR5 = GL_COLOR_ATTACHMENT5,
		ATTACHMENT_COLOR6 = GL_COLOR_ATTACHMENT6,
		ATTACHMENT_COLOR7 = GL_COLOR_ATTACHMENT7,
		ATTACHMENT_COLOR8 = GL_COLOR_ATTACHMENT8,
		ATTACHMENT_COLOR9 = GL_COLOR_ATTACHMENT9,
		ATTACHMENT_COLOR10 = GL_COLOR_ATTACHMENT10,
		ATTACHMENT_COLOR11 = GL_COLOR_ATTACHMENT11,
		ATTACHMENT_COLOR12 = GL_COLOR_ATTACHMENT12,
		ATTACHMENT_COLOR13 = GL_COLOR_ATTACHMENT13,
		ATTACHMENT_COLOR14 = GL_COLOR_ATTACHMENT14,
		ATTACHMENT_COLOR15 = GL_COLOR_ATTACHMENT15,
		ATTACHMENT_NONE = GL_NONE,
	};

	class FBO {
	public:
		
		FBO();
		~FBO();
		
		void Destroy();
		
		void AttachTexture(Texture* texture, FboAttachmentType  attachmentType, uint32_t bindLocation);
		void DetachTexture(FboAttachmentType  attachmentType);
		void AttachColor(Texture* texture, int colorId, uint32_t bindLocation);
		void DetachColor(int colorId);
		void AttachDepth(Texture* texture);
		void DetachDepth();
		
		void Viewport(int x, int y, int width, int height);
		void Clear(bool color, bool depth);
		void SetClearColor(glm::vec4 clearColor);
		
		void SimpleBind();
		void Bind();
		static void Unbind();
		
		GLenum CheckStatus();
		
		inline uint32_t FboId() const { return fbo; }
		
	private:
		
		uint32_t fbo;
		uint32_t x, y, width, height;
		glm::vec4 clearColor;
		
		std::vector<FboAttachmentType> attachmentBuffers;
		
		
		static FBO* currentlyBoundFBO;
	};
}

#endif

