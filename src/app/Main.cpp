
#include "../OpenGLWrapper/include/openglwrapper/OpenGL.hpp"

#include "../include/boltzmann/NeuralNetwork.hpp"

float Test(float a, float b, bn::NeuralNetwork& nn) {
	float x[2]={a,b}, y[5];
	nn.UpdateStates(x, 0, 2);
	nn.PerformCalculation(0, 5);
	nn.SwapStates();
	nn.PerformCalculation(0, 5);
	nn.FetchStates(y, 0, 5);
	nn.SwapStates();
	return y[4];
}

void Print(float a, float b, bn::NeuralNetwork& nn) {
	float y = Test(a, b, nn);
	printf(" %2.3f %2.3f -> %2.3f\n", a, b, y);
}

int main() {
	gl::openGL.InitHeadless();
	
	bn::NeuralNetwork nn;
	nn.InitEmptyNetwork({{}, {}, {0,1}, {0, 1}, {2,3}});
	float bias[] = {0.f,0.f,-10.f, -10, 10};
	float weight[] = {10.f, -10.f, -10.f, 10.f, 10, 10};
	nn.UpdateBiasWeights(bias, weight);
	
	Print(-1, -1, nn);
	Print(-1, +1, nn);
	Print(+1, -1, nn);
	Print(+1, +1, nn);
	
	gl::openGL.Destroy();
	
	return 0;
}

