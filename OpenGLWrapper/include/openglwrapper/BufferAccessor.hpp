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

#ifndef BUFFER_ACCESSOR_H
#define BUFFER_ACCESSOR_H

#include <vector>
#include <tuple>

#include "VBO.hpp"

namespace gl {

	template<typename T, uint32_t C>
	class Atr {
	public:
		using type = T;
		const static uint32_t elements = C;
		const static uint32_t size = C*sizeof(T);
	};
	
	template<typename T, uint32_t C>
	using A = Atr<T, C>;
	
namespace BufferAccessor {

	/*
		usage:
		
		VBO vbo(...);
		auto buf = vbo.Buffer<Atr<float, 3>, Atr<float, 2>, Atr<uint32_t short, 2>>();
		buf.At<0>(i, 0) = value;
		
	*/
	
	template<uint32_t id, typename tuple>
	class GetOffset {
	public:
		const static uint32_t offset =
			GetOffset<id-1, tuple>::offset +
			std::tuple_element<id-1, tuple>::type::size;
	};
	
	template<typename tuple>
	class GetOffset<0, tuple> {
	public:
		const static uint32_t offset = 0;
	};
	
	template<typename... TupleArgs>
	class BufferRef {
	public:
		
		using Tuple = std::tuple<TupleArgs...>;
		
		BufferRef(VBO* vbo, std::vector<uint8_t>& buf) : buffer(buf), vertexSize(vbo->VertexSize()) {}
		BufferRef(BufferRef&& r) = default;
		BufferRef(BufferRef& r) : buffer(r.buffer), vertexSize(r.vertexSize) {}
		
		template<uint32_t id>
		using TupleElement = typename std::tuple_element<id, Tuple>::type;
		
		template<uint32_t attributeID>
		inline typename TupleElement<attributeID>::type& At(uint32_t vertId,
				uint32_t vectorId=0) {
			uint32_t minBytes = (vertId+1)*vertexSize;
			if(buffer.size() < minBytes)
				buffer.resize(minBytes);
			size_t offset =  vertId*vertexSize +
				GetOffset<attributeID, Tuple>::offset;
			using elem_type = typename TupleElement<attributeID>::type;
			elem_type* elements =
				reinterpret_cast<elem_type*>(&(buffer[offset]));
			return elements[vectorId];
// 			static float _S[64];
// 			return *((typename TupleElement<attributeID>::type*)&(_S));
		}
		
	private:
	
		std::vector<uint8_t>& buffer;
		uint32_t vertexSize;
	};
}
}

#endif
