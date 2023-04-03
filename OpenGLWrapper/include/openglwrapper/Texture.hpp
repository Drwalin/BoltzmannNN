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

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>

#include "OpenGL.hpp"

namespace gl {
	enum TextureDataFormat : GLenum {
		RED = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RED_INTEGER = GL_RED_INTEGER,
		RG_INTEGER = GL_RG_INTEGER,
		RGB_INTEGER = GL_RGB_INTEGER,
		BGR_INTEGER = GL_BGR_INTEGER,
		RGBA_INTEGER = GL_RGBA_INTEGER,
		BGRA_INTEGER = GL_BGRA_INTEGER,
		
		STENCIL_INDEX = GL_STENCIL_INDEX,
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
		DEPTH_STENCIL = GL_DEPTH_STENCIL
	};

	enum TextureTarget : GLenum {
		// TODO: verify!!!
		TEXTURE_3D = GL_TEXTURE_3D,
		TEXTURE_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
		TEXTURE_CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
		PROXY_TEXTURE_3D = GL_PROXY_TEXTURE_3D,
		PROXY_TEXTURE_2D_ARRAY = GL_PROXY_TEXTURE_2D_ARRAY,
		PROXY_TEXTURE_CUBE_MAP_ARRAY = GL_PROXY_TEXTURE_CUBE_MAP_ARRAY,
		
		TEXTURE_2D = GL_TEXTURE_2D,
		PROXY_TEXTURE_2D = GL_PROXY_TEXTURE_2D,
		TEXTURE_1D_ARRAY = GL_TEXTURE_1D_ARRAY,
		PROXY_TEXTURE_1D_ARRAY = GL_PROXY_TEXTURE_1D_ARRAY,
		TEXTURE_RECTANGLE = GL_TEXTURE_RECTANGLE,
		PROXY_TEXTURE_RECTANGLE = GL_PROXY_TEXTURE_RECTANGLE,
		TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		TEXTURE_CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		TEXTURE_CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		TEXTURE_CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		TEXTURE_CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		TEXTURE_CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		PROXY_TEXTURE_CUBE_MAP = GL_PROXY_TEXTURE_CUBE_MAP
	};
	
	enum TextureWrapParam : GLenum {
		REPEAT = GL_REPEAT,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
		MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
		MIRROR_CLAMP_TO_EDGE = GL_MIRROR_CLAMP_TO_EDGE
	};
	
	enum TextureMinFilter : GLenum {
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};
	
	enum TextureMagFilter : GLenum {
		MAG_LINEAR = GL_LINEAR,
		MAG_NEAREST = GL_NEAREST
	};
	
	enum TextureSizedInternalFormat : GLenum {
		R8 = GL_R8,
		R8_SNORM = GL_R8_SNORM,
		R16 = GL_R16,
		R16_SNORM = GL_R16_SNORM,
		RG8 = GL_RG8,
		RG8_SNORM = GL_RG8_SNORM,
		RG16 = GL_RG16,
		RG16_SNORM = GL_RG16_SNORM,
		R3_G3_B2 = GL_R3_G3_B2,
		RGB4 = GL_RGB4,
		RGB5 = GL_RGB5,
		RGB8 = GL_RGB8,
		RGB8_SNORM = GL_RGB8_SNORM,
		RGB10 = GL_RGB10,
		RGB12 = GL_RGB12,
		RGB16_SNORM = GL_RGB16_SNORM,
		RGBA2 = GL_RGBA2,
		RGBA4 = GL_RGBA4,
		RGB5_A1 = GL_RGB5_A1,
		RGBA8 = GL_RGBA8,
		RGBA8_SNORM = GL_RGBA8_SNORM,
		RGB10_A2 = GL_RGB10_A2,
		RGB10_A2UI = GL_RGB10_A2UI,
		RGBA12 = GL_RGBA12,
		RGBA16 = GL_RGBA16,
		SRGB8 = GL_SRGB8,
		SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
		R16F = GL_R16F,
		RG16F = GL_RG16F,
		RGB16F = GL_RGB16F,
		RGBA16F = GL_RGBA16F,
		R32F = GL_R32F,
		RG32F = GL_RG32F,
		RGB32F = GL_RGB32F,
		RGBA32F = GL_RGBA32F,
		R11F_G11F_B10F = GL_R11F_G11F_B10F,
		RGB9_E5 = GL_RGB9_E5,
		R8I = GL_R8I,
		R8UI = GL_R8UI,
		R16I = GL_R16I,
		R16UI = GL_R16UI,
		R32I = GL_R32I,
		R32UI = GL_R32UI,
		RG8I = GL_RG8I,
		RG8UI = GL_RG8UI,
		RG16I = GL_RG16I,
		RG16UI = GL_RG16UI,
		RG32I = GL_RG32I,
		RG32UI = GL_RG32UI,
		RGB8I = GL_RGB8I,
		RGB8UI = GL_RGB8UI,
		RGB16I = GL_RGB16I,
		RGB16UI = GL_RGB16UI,
		RGB32I = GL_RGB32I,
		RGB32UI = GL_RGB32UI,
		RGBA8I = GL_RGBA8I,
		RGBA8UI = GL_RGBA8UI,
		RGBA16I = GL_RGBA16I,
		RGBA16UI = GL_RGBA16UI,
		RGBA32I = GL_RGBA32I,
		RGBA32UI = GL_RGBA32UI,
		
		DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
	};

	class Texture {
	private:
		
		int width, height, depth;
		uint32_t textureID;
		gl::TextureTarget target;
		
	public:
		
		inline bool Loaded() const { return textureID; }
		inline int GetWidth() const { return width; }
		inline int GetHeight() const { return height; }
		inline int GetDepth() const { return depth; }
		
		bool Load(const char* fileName, bool generateMipMap,
				int forceChannelsCount=0);		// return 0 if no errors
		bool Load(const char* fileName, bool generateMipMap,
				gl::TextureSizedInternalFormat forceSizedInternalFormat,
				int forceChannelsCount=0);		// return 0 if no errors
		
		
		void Generate1(gl::TextureTarget target,
				uint32_t w,
				gl::TextureSizedInternalFormat internalformat,
				gl::TextureDataFormat dataformat=RGBA, gl::DataType datatype=UNSIGNED_BYTE);
		void Update1(const void* pixels,
				uint32_t x,
				uint32_t w,
				uint32_t level,
				gl::TextureDataFormat dataformat, gl::DataType datatype);
		void Fetch1(void* pixels,
				uint32_t x,
				uint32_t w,
				uint32_t level,
				gl::TextureDataFormat dataformat, gl::DataType datatype,
				uint32_t pixelsBufferSize);
		
		void Generate2(gl::TextureTarget target,
				uint32_t w, uint32_t h,
				gl::TextureSizedInternalFormat internalformat,
				gl::TextureDataFormat dataformat=RGBA, gl::DataType datatype=UNSIGNED_BYTE);
		void Update2(const void* pixels,
				uint32_t x, uint32_t y,
				uint32_t w, uint32_t h,
				uint32_t level,
				gl::TextureDataFormat dataformat, gl::DataType datatype);
		void Fetch2(void* pixels,
				uint32_t x, uint32_t y,
				uint32_t w, uint32_t h,
				uint32_t level,
				gl::TextureDataFormat dataformat, gl::DataType datatype,
				uint32_t pixelsBufferSize);
		
		void Generate3(gl::TextureTarget target,
				uint32_t w, uint32_t h, uint32_t d,
				gl::TextureSizedInternalFormat internalformat,
				gl::TextureDataFormat dataformat=RGBA, gl::DataType datatype=UNSIGNED_BYTE);
		void Update3(const void* pixels,
				uint32_t x, uint32_t y, uint32_t z,
				uint32_t w, uint32_t h, uint32_t d,
				uint32_t level,
				gl::TextureDataFormat dataformat, gl::DataType datatype);
		void Fetch3(void* pixels,
				uint32_t x, uint32_t y, uint32_t z,
				uint32_t w, uint32_t h, uint32_t d,
				uint32_t level,
				gl::TextureDataFormat dataformat, gl::DataType datatype,
				uint32_t pixelsBufferSize);
		
		
		void UpdateTextureData(const void* pixels, uint32_t w, uint32_t h,
				bool generateMipMap,
				gl::TextureTarget target, gl::TextureSizedInternalFormat internalformat,
				gl::TextureDataFormat dataformat, gl::DataType datatype);
		void InitTextureEmpty(uint32_t w, uint32_t h, 
				gl::TextureTarget target, gl::TextureSizedInternalFormat internalformat);
		
		
		void GenerateMipmaps();
		
		void MinFilter(TextureMinFilter filter);
		void MagFilter(TextureMagFilter filter);
		void WrapX(TextureWrapParam param);
		void WrapY(TextureWrapParam param);
		void WrapZ(TextureWrapParam param);
		
		void SetDefaultParamPixelartClampBorderNoMipmap();
		
		void Bind() const;
		uint32_t GetTexture() const;
		void Unbind();
		
		void Destroy();
		
		static uint8_t* LoadImageData(const char* fileName, int* width,
				int* height, int* channels, int forceChannelsCount=0);
		static void FreeImageData(uint8_t* imageData);
		
		Texture();
		~Texture();
	};
}

#endif

