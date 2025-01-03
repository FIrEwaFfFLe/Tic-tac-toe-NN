#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;
map<string, vector<int> > base;
vector<vector<int> > all_wins;
vector<int> nul;
int n = 3;


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


void print(string s) {
    for (int i = 0; i < n + 2; i++) {
        for (int j = 0; j < n + 2; j++) {
            if (i == 0 || j == 0 || i == n + 1 || j == n + 1) cout << "#";
            else if (s[n * (i - 1) + j - 1] == '0') cout << ' ';
            else if (s[n * (i - 1) + j - 1] == '1') cout << 'O';
            else cout << 'X';
        }    
        cout << endl;
    }
}


int main() {

    ifstream input("database3.txt");
    string s;
    int verd, x, cur, time;
    while (input >> s >> verd >> time >> x) {
        for (int i = 0; i < x; i++) {
            input >> cur;
            base[s].push_back(cur);
        }
    }
    input.close();

    // generating all winning rows/columns/diagonals
    for (int i = 0; i < n; i++) {
        all_wins.push_back(nul);
        for (int j = 0; j < n; j++) {
            all_wins[all_wins.size() - 1].push_back(n * i + j);
        }
    }
    for (int j = 0; j < n; j++) {
        all_wins.push_back(nul);
        for (int i = 0; i < n; i++) {
            all_wins[all_wins.size() - 1].push_back(n * i + j);
        }
    }
    all_wins.push_back(nul);
    for (int i = 0; i < n; i++) {
        all_wins[all_wins.size() - 1].push_back(n * i + i);
    }
    all_wins.push_back(nul);
    for (int i = 0; i < n; i++) {
        all_wins[all_wins.size() - 1].push_back(n * (n - i - 1) + i);
    }


    while (1) {
        string position;
        int move = 0;
        for (int i = 0; i < n * n; i++) position += '0';
        print(position);
        while (position_status(position) == -1) {
            if (move % 2 == 1) {
                cin >> cur;
                position[cur] = '1';
            }
            else {
                x = base[position][(rand() % base[position].size())];
                position[x] = '2';
            }
            move++;
            print(position);
            cout << "______________________________________________" << endl;
        }
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    }


    return 0;
}