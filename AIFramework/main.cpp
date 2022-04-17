#include "Source/NeuralNetworkApplication.h"

using namespace AIFramework;

int main()
{
    srand(time(NULL));
    NeuralNetworkApplication* NNApp = new NeuralNetworkApplication;
    NNApp->Run(); 
    return 0;
}