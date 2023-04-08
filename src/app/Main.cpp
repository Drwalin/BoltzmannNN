
#include <algorithm>
#include <random>
#include <ctime>
#include <chrono>
#include <cstdio>
#include <set>

#include "../OpenGLWrapper/include/openglwrapper/OpenGL.hpp"

#include "../include/boltzmann/NeuralNetwork.hpp"

void GenerateRandomStructure(std::vector<std::vector<uint32_t>>& structure,
		uint32_t neurons, uint32_t connectionsPerNeuron,
		uint32_t inputOnlyNeurons, uint32_t connectionsWithInputNeuronsPerInput) {
	structure.resize(neurons);
	std::uniform_int_distribution<uint32_t> dist(inputOnlyNeurons, neurons-inputOnlyNeurons);
	for(uint32_t i=0; i<neurons-inputOnlyNeurons; ++i) {
		structure[i].clear();
		structure[i].reserve(connectionsPerNeuron);
	}
	std::default_random_engine gen(time(NULL));
	
	std::set<uint32_t> con;
	for(uint32_t i=0; i<inputOnlyNeurons; ++i) {
		con.clear();
		for(; con.size() < connectionsWithInputNeuronsPerInput;) {
			con.insert(dist(gen));
		}
		for(auto v : con) {
			structure[v].emplace_back(i);
		}
	}
	
	for(uint32_t i=inputOnlyNeurons; i<neurons; ++i) {
		for(; structure[i].size() < connectionsPerNeuron;) {
			uint32_t v = dist(gen);
			if(v != i)
				structure[i].emplace_back(v);
		}
	}
}

int main() {
	gl::openGL.InitHeadless();
	
	{
		constexpr uint32_t NEURONS=1024*1024;
		constexpr uint32_t CONNECTIONS_PER_NEURON=128;
		
		auto t1 = std::chrono::steady_clock::now();
		
		bn::NeuralNetwork nn;
		std::vector<std::vector<uint32_t>> structure;
		GenerateRandomStructure(structure,
				NEURONS, CONNECTIONS_PER_NEURON, 64, NEURONS/64);
		nn.InitEmptyNetwork(structure);
		
		auto t2 = std::chrono::steady_clock::now();
		printf(" network generation time: %.3f ms\n", (t2-t1).count()/1000000.0f);
		t2 = std::chrono::steady_clock::now();
		constexpr uint32_t ITERATIONS = 256;
		
		for(int i=0; i<ITERATIONS; ++i) {
			nn.PerformCalculation(0, NEURONS);
			glFinish();
		}
		
		auto t3 = std::chrono::steady_clock::now();
		printf(" One iteration time: %.3f ms\n", (t3-t2).count()/1000.0f/1000.f/ITERATIONS);
		printf(" Calculation time per neuron: %.3f ns\n", (t3-t2).count()/(float)(ITERATIONS*NEURONS));
	}
	gl::openGL.Destroy();
	
	return 0;
}

