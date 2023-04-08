/*
 *  This file is part of BoltzmannNN
 *  Copyright (C) 2023 Marek Zalewski aka Drwalin
 *
 *  BoltzmannNN is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  BoltzmannNN is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BOLTZMANNNN_NEURAL_NETWORK_HPP
#define BOLTZMANNNN_NEURAL_NETWORK_HPP

#include <vector>

#include "../../OpenGLWrapper/include/openglwrapper/VBO.hpp"
#include "../../OpenGLWrapper/include/openglwrapper/Shader.hpp"

namespace gl {
	template<typename T>
	class SimpleVBO : public gl::VBO {
	public:
		SimpleVBO() : gl::VBO(sizeof(T), gl::ARRAY_BUFFER, gl::DYNAMIC_DRAW) {}
		
		void UpdateElements(const T* data, uint32_t start, uint32_t count) {
			if(start+count > this->GetVertexCount())
				count = this->GetVertexCount()-start;
			Update(data, start*sizeof(T), count*sizeof(T));
		}
		
		void FetchElements(T* data, uint32_t start, uint32_t count) {
			if(start+count > this->GetVertexCount())
				count = this->GetVertexCount()-start;
			Fetch(data, start*sizeof(T), count*sizeof(T));
		}
	};
}

namespace bn {
	class NeuralNetwork {
	public:
		
		NeuralNetwork();
		~NeuralNetwork();
		
		void InitEmptyNetwork(const std::vector<std::vector<uint32_t>>& structure);
		
		void SwapStates();
		
		void UpdateStates(const float* data, uint32_t start, uint32_t elements);
		void FetchStates(float* data, uint32_t start, uint32_t elements);
		
		void PerformCalculation(uint32_t start, uint32_t count);
		
	private:
		
		struct PerNeuronStatic {
			uint32_t weights_start; // position of bias
			uint32_t weights_count; // excluding bias. if no weights are
									// present, then no bias is present either
		};
		struct WeightInfo {
			uint32_t neuronId;
			float weight;
		};
		
		uint32_t weightsAndBiasesCount, neuronsCount;
		
		std::vector<std::vector<uint32_t>> structure;
		
		
		gl::SimpleVBO<float> *statePrevious, *stateNext;
		gl::SimpleVBO<float> states[2];
		gl::SimpleVBO<float> weightsStructure; // [ bias[0], weight[0][0],
											   // weight[0][1], ...,
											   // weight[0][n-1], bias[1],
											   // weight[1][0], ...
		gl::SimpleVBO<PerNeuronStatic> perNeuronStatic;
		
		std::vector<PerNeuronStatic> perNeuronStaticInfoHost;
		
		gl::Shader calculation;
		
	private:
		
		const static char* CALCULATIONS_SOURCE_CODE;
	};
}

#endif

