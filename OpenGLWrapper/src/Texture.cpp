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

#include "../include/openglwrapper/Texture.hpp"

#include "../thirdparty/SOIL2/src/SOIL2/SOIL2.h"

#include <cstdio>
#include <string>

namespace gl {

Texture::Texture() {
	textureID = 0;
	width = 0;
	height = 0;
	depth = 0;
}

Texture::~Texture() {
	Destroy();
}

bool Texture::Load(const char* fileName,
		bool generateMipMap, int forceChannelsCount) {
	return Load(fileName, generateMipMap,
			(gl::TextureSizedInternalFormat)gl::RGBA, forceChannelsCount);
}

bool Texture::Load(const char* fileName, bool generateMipMap,
		gl::TextureSizedInternalFormat forceSizedInternalFormat,
		int forceChannelsCount) {
	int channels = 0;
	int32_t w, h;
	uint8_t * image = LoadImageData(fileName, &w, &h, &channels,
			forceChannelsCount);
	if(image==NULL && textureID) {
		glDeleteTextures(1, &textureID);
		textureID = width = height = depth = 0;
		return false;
	}
	
	TextureDataFormat format = RGBA;
	switch(forceChannelsCount ? forceChannelsCount : channels) {
		case 1: format = RED; break;
		case 2: format = RG; break;
		case 3: format = RGB; break;
		case 4: format = RGBA; break;
		default:
			glDeleteTextures(1, &textureID);
			textureID = width = height = 0;
			return false;
	}
	
	Generate2(TEXTURE_2D, w, h, forceSizedInternalFormat);
	Update2(image, 0, 0, w, h, 0, format, gl::UNSIGNED_BYTE);
	if(generateMipMap)
		GenerateMipmaps();
	
	FreeImageData(image);
	depth = 1;
	return true;
}



void Texture::Generate1(gl::TextureTarget target,
		uint32_t w,
		gl::TextureSizedInternalFormat internalformat,
		gl::TextureDataFormat dataformat, gl::DataType datatype) {
	if(textureID && target != this->target) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	GL_CHECK_PUSH_PRINT_ERROR;
	
	if(!textureID)
		glCreateTextures(target, 1, &textureID);
	GL_CHECK_PUSH_PRINT_ERROR;
	this->target = target;
	glBindTexture(target, textureID);
	GL_CHECK_PUSH_PRINT_ERROR;
	
	this->width = w;
	this->height = 1;
	this->depth = 1;
	
	glTexImage1D(target, 0, internalformat, w, 0,
			dataformat, datatype, NULL);
	GL_CHECK_PUSH_PRINT_ERROR;
	
	MinFilter(gl::NEAREST);
	GL_CHECK_PUSH_PRINT_ERROR;
}

void Texture::Update1(const void* pixels,
		uint32_t x,
		uint32_t w,
		uint32_t level,
		gl::TextureDataFormat dataformat, gl::DataType datatype) {
	glTextureSubImage1D(textureID, level, x, w,
			dataformat, datatype, pixels);
	GL_CHECK_PUSH_PRINT_ERROR;
}

void Texture::Fetch1(void* pixels,
		uint32_t x,
		uint32_t w,
		uint32_t level,
		gl::TextureDataFormat dataformat, gl::DataType datatype,
		uint32_t pixelsBufferSize) {
	glGetTextureSubImage(textureID, level, x, 0, 0, w, 1, 1,
			dataformat, datatype, pixelsBufferSize, pixels);
	GL_CHECK_PUSH_PRINT_ERROR;
}


void Texture::Generate2(gl::TextureTarget target,
		uint32_t w, uint32_t h,
		gl::TextureSizedInternalFormat internalformat,
		gl::TextureDataFormat dataformat, gl::DataType datatype) {
	if(textureID && target != this->target) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	GL_CHECK_PUSH_PRINT_ERROR;
	
	this->target = target;
	if(!textureID)
		glCreateTextures(target, 1, &textureID);
	GL_CHECK_PUSH_PRINT_ERROR;
	glBindTexture(target, textureID);
	GL_CHECK_PUSH_PRINT_ERROR;
	
	this->width = w;
	this->height = h;
	this->depth = 1;
	
	
	glTexImage2D(target, 0, internalformat, w, h, 0,
			dataformat, datatype, NULL);
	GL_CHECK_PUSH_PRINT_ERROR;
	
	MinFilter(gl::NEAREST);
	MagFilter(gl::MAG_NEAREST);
	GL_CHECK_PUSH_PRINT_ERROR;
}

void Texture::Update2(const void* pixels,
		uint32_t x, uint32_t y,
		uint32_t w, uint32_t h,
		uint32_t level,
		gl::TextureDataFormat dataformat, gl::DataType datatype) {
	glTextureSubImage2D(textureID, level, x, y, w, h,
			dataformat, datatype, pixels);
	GL_CHECK_PUSH_PRINT_ERROR;
}

void Texture::Fetch2(void* pixels,
		uint32_t x, uint32_t y,
		uint32_t w, uint32_t h,
		uint32_t level,
		gl::TextureDataFormat dataformat, gl::DataType datatype,
		uint32_t pixelsBufferSize) {
	glGetTextureSubImage(textureID, level, x, y, 0, w, h, 1,
			dataformat, datatype, pixelsBufferSize, pixels);
	GL_CHECK_PUSH_PRINT_ERROR;
}


void Texture::Generate3(gl::TextureTarget target,
		uint32_t w, uint32_t h, uint32_t d,
		gl::TextureSizedInternalFormat internalformat,
		gl::TextureDataFormat dataformat, gl::DataType datatype) {
	if(textureID && target != this->target) {
		glDeleteTextures(1, &textureID);
		textureID = 0;
	}
	GL_CHECK_PUSH_PRINT_ERROR;
	
	if(!textureID)
		glCreateTextures(target, 1, &textureID);
	GL_CHECK_PUSH_PRINT_ERROR;
	this->target = target;
	glBindTexture(target, textureID);
	GL_CHECK_PUSH_PRINT_ERROR;
	
	this->width = w;
	this->height = h;
	this->depth = d;
	
	glTexImage3D(target, 0, internalformat, w, h, d, 0,
			dataformat, datatype, NULL);
	GL_CHECK_PUSH_PRINT_ERROR;
	
	MinFilter(gl::NEAREST);
	MagFilter(gl::MAG_NEAREST);
	GL_CHECK_PUSH_PRINT_ERROR;
}

void Texture::Update3(const void* pixels,
		uint32_t x, uint32_t y, uint32_t z,
		uint32_t w, uint32_t h, uint32_t d,
		uint32_t level,
		gl::TextureDataFormat dataformat, gl::DataType datatype) {
	glTextureSubImage3D(textureID, level, x, y, z, w, h, d,
			dataformat, datatype, pixels);
	GL_CHECK_PUSH_PRINT_ERROR;
}

void Texture::Fetch3(void* pixels,
		uint32_t x, uint32_t y, uint32_t z,
		uint32_t w, uint32_t h, uint32_t d,
		uint32_t level,
		gl::TextureDataFormat dataformat, gl::DataType datatype,
		uint32_t pixelsBufferSize) {
	glGetTextureSubImage(textureID, level, x, y, z, w, h, d,
			dataformat, datatype, pixelsBufferSize, pixels);
	GL_CHECK_PUSH_PRINT_ERROR;
}



void Texture::UpdateTextureData(const void* pixels, uint32_t w, uint32_t h,
		bool generateMipMap,
		gl::TextureTarget target,
		gl::TextureSizedInternalFormat internalformat,
		gl::TextureDataFormat dataformat,
		gl::DataType datatype) {
	Generate2(target, w, h, internalformat, dataformat, datatype);
	if(pixels) {
		Update2(pixels, 0, 0, w, h, 0, dataformat, datatype);
	}
	
	if(generateMipMap) {
		GenerateMipmaps();
	} else {
		MinFilter(gl::NEAREST);
		MagFilter(gl::MAG_NEAREST);
	}
}

void Texture::InitTextureEmpty(uint32_t w, uint32_t h, 
		gl::TextureTarget target, gl::TextureSizedInternalFormat internalformat) {
	UpdateTextureData(NULL, w, h, false, target, internalformat, gl::RGBA, gl::UNSIGNED_BYTE);
}



void Texture::GenerateMipmaps() {
	glGenerateTextureMipmap(textureID);
}

void Texture::MinFilter(TextureMinFilter filter) {
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, filter);
}

void Texture::MagFilter(TextureMagFilter filter) {
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, filter);
}

void Texture::WrapX(TextureWrapParam param) {
	glTexParameteri(target, GL_TEXTURE_WRAP_S, param);
}

void Texture::WrapY(TextureWrapParam param) {
	glTexParameteri(target, GL_TEXTURE_WRAP_T, param);
}

void Texture::WrapZ(TextureWrapParam param) {
	glTexParameteri(target, GL_TEXTURE_WRAP_R, param);
}

void Texture::SetDefaultParamPixelartClampBorderNoMipmap() {
	WrapX(gl::MIRRORED_REPEAT);
	WrapY(gl::MIRRORED_REPEAT);
	MinFilter(gl::NEAREST);
	MagFilter(gl::MAG_NEAREST);
}

void Texture::Bind() const {
	glBindTexture(target, textureID);
}

uint32_t Texture::GetTexture() const {
	return textureID;
}

void Texture::Unbind() {
	glBindTexture(target, 0);
}

void Texture::Destroy() {
	if(textureID) {
		glDeleteTextures(1, &textureID);
		width = 0;
		height = 0;
		textureID = 0;
	}
}

uint8_t* Texture::LoadImageData(const char* fileName, int* width, int* height,
		int* channels, int forceChannelsCount) {
	return SOIL_load_image(fileName, width, height, channels,
			forceChannelsCount);
}

void Texture::FreeImageData(uint8_t* imageData) {
	SOIL_free_image_data(imageData);
}

}

