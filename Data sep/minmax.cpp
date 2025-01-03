#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

struct pos_char {
    int res;
    int time;
    vector<int> moves;
};


map<string, pos_char> base;
vector<vector<int>> all_wins;
int n = 4;
const int inf = 2000000000;


int position_status(string s) {
    // -1 - game hasn't ended, 0 - draw, 1 - opponent (1) won, 2 - we (2) won.

    for (auto cur : all_wins) {
        bool all_equal = true;
        for (int i = 1; i < cur.size(); i++) {
            if (s[cur[i]] != s[cur[i - 1]]) {
                all_equal = false; break;
            }
        }
        if (all_equal && s[cur[0]] != '0') return (s[cur[0]] - '0');
    }

    int cnt = 0;
    for (auto i : s) {
        if (i != '0') cnt++;
    }
    if (cnt == n * n)
        return 0;
    
    return -1;
}


pair<int, int> minmax(string s, int moving) {

    if (moving == 2) {
        auto it = base.find(s);
        if (it != base.end()) {
            return {(*it).second.res, (*it).second.time};
        }
    }
    else {
        string s_flip;
        for (auto i : s) {
            if (i == '1') s_flip += '2';
            else if (i == '2') s_flip += '1';
            else s_flip += i;
        }
        auto it = base.find(s_flip);
        if (it != base.end()) {
            if ((*it).second.res == 0) return {0, (*it).second.time};
            else return {3 - (*it).second.res, (*it).second.time};
        }
    }

    int stat = position_status(s);
    if (stat != -1) return {stat, 0};

    vector<vector<pair<int, int>>> available_moves(3);

    for (int i = 0; i < n * n; i++) {
        if (s[i] != '0') continue;
        string s_cur = s;
        s_cur[i] = (char)('0' + moving);
        pair<int, int> cur_res = minmax(s_cur, 3 - moving);
        available_moves[cur_res.first].emplace_back(i, cur_res.second + 1);
    }

    int verdict;
    int current_best_time = inf;

    if (available_moves[moving].size() > 0) {
        verdict = moving;
        for (auto i : available_moves[verdict]) 
            current_best_time = min(current_best_time, i.second);
    }
    else if (available_moves[0].size() > 0) {
        verdict = 0;
        current_best_time = -inf;
        for (auto i : available_moves[verdict]) 
            current_best_time = max(current_best_time, i.second);
    } 
    else {
        verdict = 3 - moving;
        current_best_time = -inf;
        for (auto i : available_moves[verdict]) 
            current_best_time = max(current_best_time, i.second);
    }

    if (moving == 2) {
        pos_char current;
        current.res = verdict;
        current.time = current_best_time;
        for (auto i : available_moves[verdict]) {
            if (i.second == current.time) {
                current.moves.push_back(i.first);
            }
        }
        base[s] = current;
    }
    else {
        pos_char current;
        if (verdict == 0) current.res = 0;
        else current.res = 3 - verdict;
        current.time = current_best_time;
        for (auto i : available_moves[verdict]) {
            if (i.second == current.time) {
                current.moves.push_back(i.first);
            }
        }
        string s_flip;
        for (auto i : s) {
            if (i == '1') s_flip += '2';
            else if (i == '2') s_flip += '1';
            else s_flip += i;
        }
        base[s_flip] = current;
    }
    return {verdict, current_best_time};

}


void end_program() {
    ofstream output("database4.txt", ios::trunc);
    for (auto i : base) {
        output << i.first << " " << i.second.res << " " << i.second.time << " " << i.second.moves.size() << " ";
        for (auto j : i.second.moves) {
            output << j << " ";
        }
        output << endl;
    }
    output.close();
}


int main() {

    // generating all winning rows/columns/diagonals
    for (int i = 0; i < n; i++) {
        all_wins.push_back({});
        for (int j = 0; j < n; j++) {
            all_wins[all_wins.size() - 1].push_back(n * i + j);
        }
    }
    for (int j = 0; j < n; j++) {
        all_wins.push_back({});
        for (int i = 0; i < n; i++) {
            all_wins[all_wins.size() - 1].push_back(n * i + j);
        }
    }
    all_wins.push_back({});
    for (int i = 0; i < n; i++) {
        all_wins[all_wins.size() - 1].push_back(n * i + i);
    }
    all_wins.push_back({});
    for (int i = 0; i < n; i++) {
        all_wins[all_wins.size() - 1].push_back(n * (n - i - 1) + i);
    }

    // generating all positions
    string zero;
    for (int i = 0; i < n * n; i++) zero += '0';
    pair<int, int> all = minmax(zero, 1);
    cout << all.first << " " << all.second << endl;
    
    end_program();


    return 0;
}