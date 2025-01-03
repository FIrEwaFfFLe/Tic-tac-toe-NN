#include <iostream>
#include <vector>
#include <memory>
#include "tools.h"
using namespace std;
using mat = vector<vector<float>>;
using matrix = shared_ptr<vector<vector<float>>>;
using arr = shared_ptr<vector<float>>;
const float zero = 0;
int n;
vector<arr> z, z_grad;
vector<matrix> w, w_grad;


void push(arr inp) {

    z[0] = inp;

    z[1] = w[0] * z[0];

    for (int i = 2; i < n; i++) {
        z[i] = w[i - 1] * ReLU(z[i - 1]);
    }
}


pair<float, shared_ptr<vector<matrix>>> calculate(int layer_numer, shared_ptr<vector<matrix>> weightss, arr input, arr output) {
    n = layer_numer;
    w = *weightss;

    z.resize(n);
    z_grad.resize(n);
    w_grad.resize(n - 1);

    push(input);

    arr nn_result = SOFTMAX(z[n - 1]);

    z_grad[n - 1] = SOFTMAX_CCE_der(nn_result, output);

    for (int i = n - 2; i >= 0; i--) {
        z_grad[i] = ReLU_der(z[i]) ^ (T(w[i]) * z_grad[i + 1]);
        (*z_grad[i])[(z_grad[i]->size()) - 1] = zero;
    }

    for (int i = n - 2; i >= 0; i--) {
        w_grad[i] = z_grad[i + 1] & ReLU(z[i]);
    }

    return {cost_CCE(nn_result, output), make_shared<vector<matrix>>(w_grad)};
}

/*
int main() {

    arr a0 = make_shared<vector<float>>(vector<float>(16, 1));
    matrix w1 = make_shared<mat>(mat(16, vector<float>(16, 1)));
    matrix w2 = make_shared<mat>(mat(16, vector<float>(16, 1)));
    
    for (int qw = 0; qw < 10000; qw++) {
        auto qwe = calculate(
            3,
            make_shared<vector<int>>(vector<int>{16, 16, 16}),
            make_shared<vector<matrix>>(vector<matrix>{w1, w2}),
            a0,
            a0
        );
    }
    

    return 0;
}

*/