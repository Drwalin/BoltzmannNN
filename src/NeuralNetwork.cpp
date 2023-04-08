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

#include <algorithm>
#include <set>
#include <random>

#include "../include/boltzmann/NeuralNetwork.hpp"
#include "openglwrapper/VBO.hpp"

namespace bn {
	void RandomBuffer(std::vector<float>& buf, uint32_t count, float min,
			float max) {
		buf.resize(count);
		static std::mt19937_64 mt;
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
		uint32_t weightsCount = 0;
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
		
		perNeuronStatic.UpdateElements(perNeuronStaticInfoHost.data(), 0,
				neuronsCount);
		
		std::vector<float> randomBuffer;
		RandomBu
		for(
		
		states[0].Resize(neuronsCount);
		states[1].Resize(neuronsCount);
		
		weightsStructure.Resize(weightsCount);
		weights.Resize(weightsCount);
		
		bias.Resize(neuronsCount);
		
		statePrevious = states;
		stateNext = states+1;
	}
	
	void NeuralNetwork::SwapStates() {
		std::swap(statePrevious, stateNext);
	}
	
	void NeuralNetwork::UpdateStates(const float* data, uint32_t start,
			uint32_t elements) {
		states->UpdateElements(data, start, elements);
	}
	
	void NeuralNetwork::FetchStates(float* data, uint32_t start,
			uint32_t elements) {
		states->FetchElements(data, start, elements);
	}

	
	void NeuralNetwork::PerformCalculation(uint32_t start, uint32_t count) {
		calculationShader.Use();
		calculationShader.SetUInt(1,
				{start, std::min(neuronsCount-start, count)});

		statePrevious->BindBufferBase(gl::SHADER_STORAGE_BUFFER, 4);
		stateNext->BindBufferBase(gl::SHADER_STORAGE_BUFFER, 5);
		
		perNeuronStatic.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 5);
		weights.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 5);
		bias.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 5);
		weightsStructure.BindBufferBase(gl::SHADER_STORAGE_BUFFER, 5);
		
		calculationShader.Dispatch(count, 1, 1);
	}


	const char* NeuralNetwork::CALCULATIONS_SOURCE_CODE = R"(#version 450 core
layout (location=1) uniform uvec2 neuronsStartCount;

struct NeuronStructureInfo {
	uint start;
	uint count;
};

layout (packed, std340, binding=1) readonly buffer Biases {
	float biases[];
};

layout (packed, std340, binding=2) readonly buffer Weights {
	float weights[];
};

layout (packed, std340, binding=3) readonly buffer NeuronsStructure {
	NeuronStructureInfo neuronStructure[];
};

layout (packed, std340, binding=4) readonly buffer StatePrevious {
	float x[];
};

layout (packed, std340, binding=5) writeonly buffer StateNext {
	float y[];
};

layout (packed, std340, binding=5) readonly buffer ConnectedNeurons {
	uint connectedNeurons[];
};

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
		sum = weights[info.start+i] * x[connectedNeurons[info.start+i]];
	}
	
	y[neuron] = tanh(sum);
})";

}

