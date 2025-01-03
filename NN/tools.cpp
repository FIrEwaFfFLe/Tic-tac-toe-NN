#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include "tools.h"
using namespace std;
using mat = vector<vector<float>>;
using matrix = shared_ptr<vector<vector<float>>>;
using arr = shared_ptr<vector<float>>;
const float zero = 0.0, one = 1.0;


matrix operator*(const matrix& y, const matrix& x) {
    // x is multiplied by y, so actually this is y * x
    int x_l = x->size(), x_w = (*x)[0].size();
    int y_l = y->size(), y_w = (*y)[0].size();

    if (x_w != y_l) {
        cout << "Not alligned matricies *";
        exit(0);
    }

    matrix ans = make_shared<mat>(x_l, vector<float>(y_w, zero));

    for (int i = 0; i < x_l; i++) {
        for (int j = 0; j < y_w; j++) {
            for (int k = 0; k < x_w; k++) {
                (*ans)[i][j] += (*x)[i][k] * (*y)[k][j];
            }
        }
    }

    return ans;
}


arr operator*(const matrix& y, const arr& x) {
    int x_l = 1, x_w = x->size();
    int y_l = y->size(), y_w = (*y)[0].size();

    if (x_w != y_l) {
        cout << "Not alligned matricies **";
        exit(0);
    }

    arr ans = make_shared<vector<float>>(y_w, zero);

    for (int j = 0; j < y_w; j++) {
        for (int k = 0; k < x_w; k++) {
            (*ans)[j] += (*x)[k] * (*y)[k][j];
        }
    }

    return ans;
}


matrix operator+(const matrix& x, const matrix& y) {
    int x_l = x->size(), x_w = (*x)[0].size();
    int y_l = y->size(), y_w = (*y)[0].size();

    if (x_l != y_l || x_w != y_w) {
        cout << "Not alligned matricies +";
        exit(0);
    }

    matrix ans = make_shared<mat>(x_l, vector<float>(x_w));

    for (int i = 0; i < x_l; i++) {
        for (int j = 0; j < x_w; j++) {
            (*ans)[i][j] = (*x)[i][j] + (*y)[i][j];
        }
    }

    return ans;
}


shared_ptr<vector<matrix>> operator+(const shared_ptr<vector<matrix>>& x, const shared_ptr<vector<matrix>>& y) {
    int x_l = x->size(), y_l = y->size();

    if (x_l != y_l) {
        cout << "Not alligned shared_ptr<vector<matrix>> +";
        exit(0);
    }

    shared_ptr<vector<matrix>> ans = make_shared<vector<matrix>>(x_l);

    for (int i = 0; i < x_l; i++) {
        (*ans)[i] = (*x)[i] + (*y)[i];
    }

    return ans;
}


matrix operator*(const matrix& x, const float& y) {
    int x_l = x->size(), x_w = (*x)[0].size();

    matrix ans = make_shared<mat>(x_l, vector<float>(x_w));

    for (int i = 0; i < x_l; i++) {
        for (int j = 0; j < x_w; j++) {
            (*ans)[i][j] = (*x)[i][j] * y;
        }
    }

    return ans;
}


shared_ptr<vector<matrix>> operator*(const shared_ptr<vector<matrix>>& x, const float& y) {
    int x_l = x->size();

    shared_ptr<vector<matrix>> ans = make_shared<vector<matrix>>(x_l);

    for (int i = 0; i < x_l; i++) {
        (*ans)[i] = (*x)[i] * y;
    }

    return ans;
}


matrix operator^(const matrix& x, const matrix& y) {
    // element by element multiplication
    int x_l = x->size(), x_w = (*x)[0].size();
    int y_l = y->size(), y_w = (*y)[0].size();

    if (x_w != y_w || x_l != y_l) {
        cout << "Not alligned matricies ^";
        exit(0);
    }

    matrix ans = make_shared<mat>(x_l, vector<float>(x_l, zero));

    for (int i = 0; i < x_l; i++) {
        for (int j = 0; j < x_w; j++) {
            (*ans)[i][j] = (*x)[i][j] * (*y)[i][j];
        }
    }

    return ans;
}


arr operator^(const arr& x, const arr& y) {
    // element by element multiplication
    int x_l = x->size();
    int y_l = y->size();

    if (x_l != y_l) {
        cout << "Not alligned matricies ^arr";
        exit(0);
    }

    arr ans = make_shared<vector<float>>(x_l);

    for (int i = 0; i < x_l; i++) {
        (*ans)[i] = (*x)[i] * (*y)[i];
    }

    return ans;
}


matrix operator&(const arr&y, const arr& x) {
    //table multiplication, y(n) & x(n) -> ans(n * n), ans[i][j] = x[i] * y[j];
    int x_l = x->size();
    int y_l = y->size();

    matrix ans = make_shared<mat>(x_l, vector<float>(y_l));

    for (int i = 0; i < x_l; i++) {
        for (int j = 0; j < y_l; j++) {
            (*ans)[i][j] = (*x)[i] * (*y)[j];
        }
    }

    return ans;
}


matrix T(matrix x) {
    // transpose
    int x_l = x->size(), x_w = (*x)[0].size();

    matrix ans = make_shared<mat>(x_w, vector<float>(x_l));

    for (int i = 0; i < x_l; i++) {
        for (int j = 0; j < x_w; j++) {
            (*ans)[j][i] = (*x)[i][j];
        }
    }

    return ans;
}


void print(arr x) {
    cout << "{";
    for (int i = 0; i < x->size() - 1; i++) {
        cout << (*x)[i] << ", ";
    }
    cout << (*x)[x->size() - 1] << "}" << endl;
}


void print(matrix x) {
    cout << "{";
    for (int i = 0; i < x->size(); i++) {
        cout << "{";
        for (int j = 0; j < (*x)[i].size(); j++) {
            cout << (*x)[i][j];
            if (j != (*x)[i].size() - 1) cout << ", ";
        }
        cout << "}";
        if (i != (x->size()) - 1) cout << ", " << endl;
    }
    cout << "}" << endl;
}


arr ReLU(arr x) {
    arr ans = make_shared<vector<float>>(x->size());
    //ReLU
    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = max((*x)[i], zero);
    }
    return ans;
}


arr ReLU_der(arr x) {
    // activation derivative
    arr ans = make_shared<vector<float>>(x->size());
    //ReLU
    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = ((*x)[i] >= zero ? one : zero);
    }
    return ans;
}


arr SIGM(arr x) {
    arr ans = make_shared<vector<float>>(x->size());

    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = 1 / (1 + exp(-(*x)[i]));
    }
    return ans;
}


arr SIGM_der(arr x) {
    arr ans = make_shared<vector<float>>(x->size());

    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = exp((*x)[i]) / pow(1 + exp((*x)[i]), 2);
    }

    return ans;
}


arr SOFTMAX(arr x) {
    arr ans = make_shared<vector<float>>(x->size());

    float su = zero;
    for (int i = 0; i < x->size(); i++) {
        su += exp((*x)[i]);
    }
    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = exp((*x)[i]) / su;
    }
    return ans;
}


float cost_MSE(arr x, arr y) {
    float ans = zero;

    for (int i = 0; i < x->size(); i++) {
        ans += ((*x)[i] - (*y)[i]) * ((*x)[i] - (*y)[i]);
    }

    return ans / 2;
}


arr cost_MSE_der(arr x, arr y) {
    arr ans = make_shared<vector<float>>(x->size());

    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = ((*x)[i] - (*y)[i]);
    }

    return ans;
}


float cost_CCE(arr x, arr y) {
    float ans = 0;

    for (int i = 0; i < x->size(); i++) {
        ans -= (*y)[i] * log((*x)[i]);
    }
    return ans;
}


arr SOFTMAX_CCE_der(arr x, arr y) {
    arr ans = make_shared<vector<float>>(x->size());

    for (int i = 0; i < x->size(); i++) {
        (*ans)[i] = ((*x)[i] - (*y)[i]);
    }

    return ans;
}
