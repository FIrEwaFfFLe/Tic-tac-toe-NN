#ifndef GRADIENT_H
#define GRADIENT_H

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>

using namespace std;
using mat = vector<vector<float>>;
using matrix = shared_ptr<vector<vector<float>>>;
using arr = shared_ptr<vector<float>>;

pair<float, shared_ptr<vector<matrix>>> calculate(int layer_numer, shared_ptr<vector<matrix>> weights, arr input, arr output);

#endif