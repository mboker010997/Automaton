#include <vector>
#include <string>
#include <bitset>
#include <set>
#include <queue>
#include <map>
#include <algorithm>
#include <iostream>

using std::vector;
using std::pair;
using std::string;
using std::bitset;
using std::cout;
using std::map;
using std::set;

class Automaton {
private:

    struct Edge {
        int v, to;
        char w;

        Edge() = default;

        Edge(int v, int to, const char& w) : v(v), to(to), w(w) {}
    };

    static const size_t MAX_VERTEX = 100;
    static const char eps = '_';

    static bool is_eps(char c);

    int start;
    int vertex;
    string alp;
    vector<vector<Edge>> edges;
    vector<map<char, int>> go;
    std::set<int> terms;

    static bool get_bit(int mask, int pos);

    vector<Edge> get_edges_list(std::set<int> removed = std::set<int>());

    void compress_and_assign_edges(const vector<Edge>& pairs);

    void remove_extra_vertices();

    void build_go();

public:
    Automaton() = default;

    Automaton(int vertex, int start, const string& alp, const vector<int>& _terms);

    int get_vertex();

    int get_start();

    string get_alp();

    int next(int v, char c);

    vector<int> get_all_next(int v, char c);

    void add_edge(int v, int to, char w);

    void remove_eps_edges();

    bool is_dka();

    void to_dka();

    void to_pdka();

    void to_mpdka();

    friend bool operator<(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator==(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator<=(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator>=(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator>(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator!=(const Automaton::Edge& fst, const Automaton::Edge& snd);

    friend std::ostream& operator<<(std::ostream& out, const Automaton& aut);
    friend std::istream& operator>>(std::istream& in, Automaton& aut);
};