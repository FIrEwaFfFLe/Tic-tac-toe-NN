#include <bits/stdc++.h>
using namespace std;
map<string, int> base;
vector<vector<int>> all_wins;
int n = 2;


bool is_valid(string s) {
    int cnt1 = 0, cnt2 = 0;
    for (char i : s) {
        if (i == '1') cnt1++;
        else if (i == '2') cnt2++;
    }

    if ((cnt1 != cnt2 && cnt1 != cnt2 + 1) || cnt1 + cnt2 == n * n) return false;

    for (auto cur : all_wins) {
        bool all_equal = true;
        for (int i = 1; i < cur.size(); i++) {
            if (s[cur[i]] != s[cur[i - 1]]) {
                all_equal = false; break;
            }
        }
        if (all_equal && s[cur[0]] != '0') return false;
    }

    return true;
}


int try_for_answer(string s) {

    // checking for immediate wins
    for (auto cur : all_wins) {
        int cur_cnt = 0;
        for (int i = 0; i < cur.size(); i++) {
            if (s[cur[i]] == '2') {
                cur_cnt++;
            }
        }
        if (cur_cnt < n - 1) continue;
        for (int i = 0; i < cur.size(); i++) {
            if (s[cur[i]] == '0') {
                return cur[i];
            }
        }
    }

    // checking for immediate opponent wins
    for (auto cur : all_wins) {
        int cur_cnt = 0;
        for (int i = 0; i < cur.size(); i++) {
            if (s[cur[i]] == '1') {
                cur_cnt++;
            }
        }
        if (cur_cnt < n - 1) continue;
        for (int i = 0; i < cur.size(); i++) {
            if (s[cur[i]] == '0') {
                return cur[i];
            }
        }
    }

    // checking if only one move opportunity
    int non_zero = 0;
    for (auto i : s) {
        if (i != '0') non_zero++;
    }
    if (non_zero == n * n - 1) {
        for (int i = 0; i < n * n; i++) {
            if (s[i] == '0') return i;
        }
    }
    return -1;

}

void print(string s, int our) {
    for (int i = 0; i < n + 2; i++) {
        for (int j = 0; j < n + 2; j++) {
            if (i == 0 || j == 0 || i == n + 1 || j == n + 1) cout << "#";
            else if (n * (i - 1) + j - 1 == our) cout << "!";
            else if (s[n * (i - 1) + j - 1] == '0') cout << ' ';
            else if (s[n * (i - 1) + j - 1] == '1') cout << 'O';
            else cout << 'X';
        }    
        cout << endl;
    }
}


int human_input(string s) {
    int inp = 0;
    while (1) {
        print(s, -1);
        cin >> inp;
        if (inp == -1) break;
        // a skip
        if (inp == 123) {
            cout << "__________________________" << base.size() << "/4536__________________________" << (float)base.size() / (float)4536 * 100 << "%" << endl;
            return 123;
        }
        
        if (inp >= n * n || s[inp] != '0') {
            cout << "Incorrect, try again." << endl; 
            cout << "__________________________" << base.size() << "/4536__________________________" << (float)base.size() / (float)4536 * 100 << "%" << endl;
            continue;
        }
        print(s, inp);
        char pass;
        cout << "[y/n]? ";
        cin >> pass;
        cout << "__________________________" << base.size() << "/4536__________________________" << (float)base.size() / (float)4536 * 100 << "%" << endl;
        if (pass == 'y') break;
    }
    return inp;
}

string next_rotation(string s) {
    string ans;
    for (int j = n - 1; j >= 0; j--) {
        for (int i = 0; i < n; i++) {
            ans += s[n * i + j];
        }
    }
    return ans;
}


string mirror(string s) {
    string ans;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            ans += s[n * i + j];
        }
    }
    return ans;
}


void end_program() {
    ofstream output("database2.txt", ios::trunc);
    for (auto i : base) {
        output << i.first << " " << i.second << endl;
    }
    output.close();
}


int main() {

    // inputing already counted positions
    
    ifstream input("database2.txt");
    string cur;
    int move;
    while (input >> cur >> move) {
        base[cur] = move;
    }
    input.close();

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
    int all_cnt = 0;
    for (int mask = 0; mask < (int)pow(3, n * n); mask++) {
        // position - out original position
        int mc = mask;
        string position;
        for (int i = 0; i < n * n; i++) {
            position += ((mc % 3) + '0');
            mc /= 3;
        }
        // valid position checking
        if (!is_valid(position) || base.find(position) != base.end()) continue;
        // at this point the position is correct and unique
        int attempt = try_for_answer(position);
        if (attempt == -1) {
            all_cnt++;
            attempt = human_input(position);
            if (attempt == -1) {
                end_program();
                return 0;
            }
            if (attempt == 123) continue;
        } 

        // all symmetrycal positions
        pair<string, int> sym_pos[8] = {};
        sym_pos[0] = {position, attempt};
        sym_pos[4] = {mirror(position), (attempt % n) * n + attempt / n};
        for (int i = 1; i < 4; i++) {
            sym_pos[i] = {next_rotation(sym_pos[i - 1].first), (n - 1 - sym_pos[i - 1].second % n) * n + sym_pos[i - 1].second / n};
            sym_pos[i + 4] = {next_rotation(sym_pos[i + 3].first), (n - 1 - sym_pos[i + 3].second % n) * n + sym_pos[i + 3].second / n};
        }
        for (int i = 0; i < 8; i++) {
            base[sym_pos[i].first] = sym_pos[i].second;
        }
    }
    cout << all_cnt << endl;
    end_program();


    return 0;
}