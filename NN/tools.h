#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <memory>
#include <vector>
#include <cmath>

using namespace std;
using mat = vector<vector<float>>;
using matrix = shared_ptr<vector<vector<float>>>;
using arr = shared_ptr<vector<float>>;

matrix operator*(const matrix& y, const matrix& x);
arr operator*(const matrix& y, const arr& x);
matrix operator+(const matrix& x, const matrix& y);
shared_ptr<vector<matrix>> operator+(const shared_ptr<vector<matrix>>& x, const shared_ptr<vector<matrix>>& y);
shared_ptr<vector<matrix>> operator*(const shared_ptr<vector<matrix>>& x, const float& y);
matrix operator*(const matrix& x, const float& y);
matrix operator^(const matrix& x, const matrix& y);
arr operator^(const arr& x, const arr& y);
matrix operator&(const arr&y, const arr& x);
matrix T(matrix x);
void print(arr x);
void print(matrix x);
arr ReLU(arr x);
arr ReLU_der(arr x);
arr SIGM(arr x);
arr SIGM_der(arr x);
arr SOFTMAX(arr x);
float cost_MSE(arr x, arr y);
arr cost_MSE_der(arr x, arr y);
float cost_CCE(arr x, arr y);
arr SOFTMAX_CCE_der(arr x, arr y);

#endif