#include <iostream>
#include "automaton.h"

using namespace std;

int main() {
    freopen("input.txt", "r", stdin);
    int n, start;
    string alp;
    cin >> n >> start;
    cin >> alp;
    vector<int> terms;
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        int el;
        cin >> el;
        terms.push_back(el);
    }
    Automaton aut(n, start, alp, terms);
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int v, to;
        char w;
        cin >> v >> to >> w;
        aut.add_edge(v, to, w);
    }
    aut.to_mpdka();
    // cout << "\n\n";
    cout << aut;
    return 0;
}