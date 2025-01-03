#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include "gradient.h"
#include "tools.h"
using namespace std;
using mat = vector<vector<float>>;
using matrix = shared_ptr<vector<vector<float>>>;
using arr = shared_ptr<vector<float>>;
const float zero = 0.0, one = 1.0, two = 2.0, mone = -1.0;
const float learning_rate = 0.01;
int user_inp1;

int layers, cur_model_num = 1;
vector<int> layer_sizes;
shared_ptr<vector<matrix>> weights;

int training_num;
vector<pair<arr, arr>> training_data;
vector<int> order;

random_device rd;
mt19937 g(rd());


void get_model(int line_number) {
    ifstream file("models.txt");

    if (!file.is_open()) {
        cout << "Couldn't open the file";
        exit(0);
    }

    string s;
    for (int i = 0; i < line_number; i++) {
        getline(file, s);
    }
    file.close();

    istringstream model(s);

    model >> layers;
    layer_sizes.resize(layers);
    weights = make_shared<vector<matrix>>(layers - 1);

    for (int i = 0; i < layers; i++) model >> layer_sizes[i];

    for (int k = 0; k < layers - 1; k++) {
        (*weights)[k] = make_shared<mat>(layer_sizes[k], vector<float>(layer_sizes[k + 1]));
        for (int i = 0; i < layer_sizes[k]; i++) {
            for (int j = 0; j < layer_sizes[k + 1]; j++) {
                model >> (*(*weights)[k])[i][j];
            }
        }
    }

    cout << "Importing model complete" << endl;
}


void write_model(float model_cost) {
    ofstream file;
    file.open("models.txt", ios_base::app);

    if (!file.is_open()) {
        cout << "Couldn't open the file in output";
        exit(0);
    }

    file << layers << " ";

    for (int i = 0; i < layers; i++) file << layer_sizes[i] << " ";

    for (int k = 0; k < layers - 1; k++) {
        for (int i = 0; i < layer_sizes[k]; i++) {
            for (int j = 0; j < layer_sizes[k + 1]; j++) {
                file << setprecision(18) << (*(*weights)[k])[i][j] << " ";
            }
        }
    }
    file << endl;
    file.close();


    file.open("comments.txt", ios_base::app);
    if (!file.is_open()) {
        cout << "Couldn't open the file in output";
        exit(0);
    }

    auto now = chrono::system_clock::now();
    time_t current_time = chrono::system_clock::to_time_t(now);
    tm* local_time = localtime(&current_time);

    file << "Model number " << cur_model_num << " with a cost of " << model_cost << ". Child of model " << user_inp1 << ". " << put_time(local_time, "%Y-%m-%d %H:%M:%S") << endl;
    cur_model_num++;
    file.close();
}


void write_cost(float model_cost) {
    ofstream file;
    file.open("log.txt", ios_base::app);

    if (!file.is_open()) {
        cout << "Couldn't open the file in write_cost";
        exit(0);
    }

    auto custom_epoch = chrono::system_clock::from_time_t(1735689600);
    auto now = chrono::system_clock::now();
    auto seconds_since_custom_epoch = chrono::duration_cast<chrono::seconds>(
        now - custom_epoch
    ).count();
    time_t current_time = chrono::system_clock::to_time_t(now);
    tm* local_time = localtime(&current_time);

    file << model_cost << " " << seconds_since_custom_epoch << "  ................................." << put_time(local_time, "%Y-%m-%d %H:%M:%S") << endl;

    file.close();
}


void import_data() {
    training_data.resize(training_num);
    order.resize(training_num);

    ifstream file("database4.txt");

    if (!file.is_open()) {
        cout << "Couldn't open the file data";
        exit(0);
    }
    string pos;
    int cnt, cur_move;
    for (int i = 0; i < training_num; i++) {
        order[i] = i;
        training_data[i].first = make_shared<vector<float>>(layer_sizes[0], zero);
        training_data[i].second = make_shared<vector<float>>(layer_sizes[layers - 1], zero);
        file >> pos;
        for (int j = 0; j < layer_sizes[0] - 1; j++) {
            if (pos[j] == '0') {
                (*training_data[i].first)[j] = 0;
            }
            else {
                (*training_data[i].first)[j] = (pos[j] - '0') * 2 - 3;
            }
        }
        (*training_data[i].first)[layer_sizes[0] - 1] = one;

        file >> cnt; file >> cnt; file >> cnt;
        for (int j = 0; j < cnt; j++) {
            file >> cur_move;
            (*training_data[i].second)[cur_move] = one / (float)cnt;
        }

        if ((i + 1) % ( 1 + training_num / 10) == 0) {
            cout << "Importing data................" << i + 1 << "/" << training_num << endl;
        }
    }
    cout << "Importing data complete" << endl;
    file.close();
}


void main_process(int batch_size) {

    int cur_batch_size, cur_test, user_inp4;
    pair<float, shared_ptr<vector<matrix>>> cur_result;
    shared_ptr<vector<matrix>> cur;
    bool first;
    float batch_cost, total_cost;

    for (int generation = 0; ; generation++) {

        shuffle(order.begin(), order.end(), g);
        total_cost = zero;

        for (int cur_batch = 0; cur_batch * batch_size < training_num; cur_batch++) {
            cur_batch_size = min(batch_size, training_num - cur_batch * batch_size);

            first = true;
            batch_cost = zero;

            for (int i = 0; i < cur_batch_size; i++) {
                cur_test = order[cur_batch * batch_size + i];

                cur_result = calculate(
                    layers, 
                    weights,
                    training_data[cur_test].first,
                    training_data[cur_test].second
                );
                batch_cost += cur_result.first;

                if (first) {
                    cur = cur_result.second;
                    first = false;
                }
                else {
                    cur = cur + cur_result.second;
                }

            }
            batch_cost /= (float)cur_batch_size;
            total_cost += batch_cost;

            weights = weights + (cur * (- learning_rate / (float)cur_batch_size));

            cout << "Batch " << generation << "." << cur_batch << " complete with cost " << batch_cost << "." << endl;
            
            if ((cur_batch + 1) % (512 / batch_size * 1400) == 0) {
                // every ~10 minutes saves
                write_model(batch_cost);
            }

            if ((cur_batch + 1) % (512 / batch_size * 140) == 0) {
                // every ~minute
                write_cost(batch_cost);
            }

        }
        total_cost /= (training_num - 1) / batch_size + 1;

        write_model(total_cost);
        
        //cout << "Continue? (0/1): ";
        //cin >> user_inp4;
        //if (user_inp4 == 0) exit(0);

    }

}


int main() {

    cout << "Import model from which line? ";
    cin >> user_inp1;
    get_model(user_inp1);
    cout << endl;
    
    cout << "Import how many training examples? (int/all): ";
    string user_inp2; cin >> user_inp2;
    if (user_inp2 == "all") training_num = 9062619;
    else training_num = stoi(user_inp2);
    import_data();
    cout << endl;

    cout << "Input batch size: ";
    int user_inp3; cin >> user_inp3;
    main_process(user_inp3);
    

    return 0;
}

/*
    auto x = calculate(
        layers, 
        make_shared<vector<int>>(layer_sizes),
        make_shared<vector<matrix>>(weights),
        make_shared<vector<float>>(vector<float>{-0.5, 1}),
        make_shared<vector<float>>(vector<float>{0.8, 0})
    );
*/