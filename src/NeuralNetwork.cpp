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

#include "../include/boltzmann/NeuralNetwork.hpp"

namespace bn {
	NeuralNetwork::NeuralNetwork() {
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
		uint32_t weightsCount = 1;
		for(uint32_t i=0; i<this->structure.size(); ++i) {
			std::vector<uint32_t>& v = this->structure[i];
			if(v.size()) {
				perNeuronStaticInfoHost[i].weights_count = v.size();
				perNeuronStaticInfoHost[i].weights_start = weightsCount;
				weightsCount += v.size() + 1;
			} else {
				perNeuronStaticInfoHost[i].weights_count = 0;
				perNeuronStaticInfoHost[i].weights_start = 0;
			}
		}
		
		perNeuronStatic.Resize(neuronsCount);
		perNeuronStatic.UpdateElements(perNeuronStaticInfoHost.data(), 0,
				neuronsCount);
		
		states[0].Resize(neuronsCount);
		states[1].Resize(neuronsCount);
		weightsStructure.Resize(weightsCount);
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

	
	void NeuralNetwork::PerformCalculation(uint32_t start, uint32_t count);


	char* NeuralNetwork::CALCULATIONS_SOURCE_CODE = R"()";
}

