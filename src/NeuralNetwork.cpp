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

#include <ctime>

#include <algorithm>
#include <set>
#include <random>

#include "openglwrapper/VBO.hpp"

#include "../include/boltzmann/NeuralNetwork.hpp"

namespace bn {
	void RandomBuffer(std::vector<float>& buf, uint32_t count, float min,
			float max) {
		buf.resize(count);
		static std::mt19937_64 mt(time(NULL));
		std::uniform_real_distribution<float> dist(min/5, max/5.0);
		for(float& v : buf) {
			v =
				+dist(mt)
				+dist(mt)
				+dist(mt)
				+dist(mt)
				+dist(mt);
		}
	}
	
	void FillBufferWithRandom(gl::SimpleVBO<float>& vbo, float min, float max) {
		std::vector<float> buf;
		RandomBuffer(buf, vbo.GetVertexCount(), min, max);
		vbo.UpdateElements(buf.data(), 0, vbo.GetVertexCount());
	}
	
	void NeuralNetwork::UpdateBiasWeights(float* bias, float* weight) {
		printf(" updating bias weights: %i %i\n", neuronsCount, weightsCount);
		weights.Update(weight, 0, weightsCount*4);
		this->bias.Update(bias, 0, neuronsCount*4);
	}
	
	
	
	NeuralNetwork::NeuralNetwork() {
		calculationShader.Compile(CALCULATIONS_SOURCE_CODE);
	}
	
	NeuralNetwork::~NeuralNetwork() {
	}
	
	void NeuralNetwork::InitEmptyNetwork(
			const std::vector<std::vector<uint32_t>>& structure) {
		neuronsCount = structure.size();
		this->structure.resize(neuronsCount);
		std::set<uint32_t> tmp;
		for(uint32_t i=0; i<neuronsCount; ++i) {
			tmp.clear();
			tmp.insert(structure[i].begin(), structure[i].end());
			this->structure[i].insert(this->structure[i].begin(), tmp.begin(),
					tmp.lower_bound(neuronsCount));
		}
		perNeuronStaticInfoHost.resize(neuronsCount);
		weightsCount = 0;
		for(uint32_t i=0; i<this->structure.size(); ++i) {
			std::vector<uint32_t>& v = this->structure[i];
			if(v.size()) {
				perNeuronStaticInfoHost[i].weights_count = v.size();
				perNeuronStaticInfoHost[i].weights_start = weightsCount;
				weightsCount += v.size();
			} else {
				perNeuronStaticInfoHost[i].weights_count = 0;
				perNeuronStaticInfoHost[i].weights_start = 0;
			}
		}
		
		weightsStructure.Resize(weightsCount);
		
		for(uint32_t i=0; i<this->structure.size(); ++i) {
			if(perNeuronStaticInfoHost[i].weights_count) {
				weightsStructure.UpdateElements(this->structure[i].data(),
						perNeuronStaticInfoHost[i].weights_start,
						perNeuronStaticInfoHost[i].weights_count);
			}
		}
		perNeuronStatic.Resize(neuronsCount);
		perNeuronStatic.UpdateElements(perNeuronStaticInfoHost.data(), 0,
				neuronsCount);
		
		states[0].Resize(neuronsCount);
		states[1].Resize(neuronsCount);
		FillBufferWithRandom(states[0], -1, 1);
		
		weights.Resize(weightsCount);
		FillBufferWithRandom(weights, -10000, 10000);
		
		bias.Resize(neuronsCount);
		FillBufferWithRandom(bias, -10000, 10000);
		
		statePrevious = states;
		stateNext = states+1;
	}
	
	void NeuralNetwork::SwapStates() {
		std::swap(statePrevious, stateNext);
	}
	
	void NeuralNetwork::UpdateStates(const float* data, uint32_t start,
			uint32_t elements) {
		statePrevious->UpdateElements(data, start, elements);
	}
	
	void NeuralNetwork::FetchStates(float* data, uint32_t start,
			uint32_t elements) {
		stateNext->FetchElements(data, start, elements);
	}

	
	void NeuralNetwork::PerformCalculation(uint32_t start, uint32_t count) {
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
		
		calculationShader.Use();
		calculationShader.SetUInt(1, start);
		calculationShader.SetUInt(2, std::min(neuronsCount-start, count));

		statePrevious->BindBufferBase(gl::SHADER_STORAGE_BUFFER, 4);
		stateNext->BindBufferBase(gl::SHADER_STORAGE_BUFFER, 5);
		
		perNeuronStatic.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 3);
		weights.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 2);
		bias.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 1);
		weightsStructure.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 6);
		
		calculationShader.DispatchRoundGroupNumbers(count, 1, 1);
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}


	const char* NeuralNetwork::CALCULATIONS_SOURCE_CODE = R"(#version 450 core
layout (location=1) uniform uint neuronsStart;
layout (location=2) uniform uint neuronsCount;

struct NeuronStructureInfo {
	uint start;
	uint count;
};

layout (packed, binding=1) readonly buffer Biases {
	float biases[];
};

layout (packed, binding=2) readonly buffer Weights {
	float weights[];
};

layout (packed, binding=3) readonly buffer NeuronsStructure {
	NeuronStructureInfo neuronStructure[];
};

layout (packed, binding=4) readonly buffer StatePrevious {
	float x[];
};

layout (packed, binding=5) writeonly buffer StateNext {
	float y[];
};

layout (packed, binding=6) readonly buffer ConnectedNeurons {
	uint connectedNeurons[];
};

layout (local_size_x = 256, local_size_y = 1, local_size_z = 1) in;

void main() {
	uint neuron = gl_GlobalInvocationID.x+neuronsStart;
	if(neuron >= neuronsCount)
		return;

	const NeuronStructureInfo info = neuronStructure[neuron];
	if(info.count == 0) {
		y[neuron] = x[neuron];
		return;
	}
	
	float sum = biases[neuron];
	for(uint i=0; i<info.count; ++i) {
		sum += weights[info.start+i] * x[connectedNeurons[info.start+i]];
	}
	
	y[neuron] = tanh(sum);
})";

}

