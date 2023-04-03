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

#ifndef BOLTZMANNNN_NEURAL_NETWORK_HPP
#define BOLTZMANNNN_NEURAL_NETWORK_HPP

#include <vector>

#include "../../OpenGLWrapper/include/openglwrapper/VBO.hpp"
#include "../../OpenGLWrapper/include/openglwrapper/Shader.hpp"

namespace bn {
	class NeuralNetwork {
	public:
		
		NeuralNetwork();
		~NeuralNetwork();
		
		void InitEmptyNetwork(const std::vector<std::vector<uint32_t>>& connections);
		
		void PerformCalculation(uint32_t start, uint32_t count);
		void SwapStates();
		
		void SetStates(float* data, uint32_t start, uint32_t count);
		void LearnWithToStates(float* data, uint32_t start, uint32_t count);
		
	private:
		
		std::vector<std::vector<uint32_t>> connections;
		
		gl::VBO *stateOld, *stateNew;
		gl::VBO *weightsStructure;
		gl::VBO *perNeuronStatic;
	};
}

#endif

