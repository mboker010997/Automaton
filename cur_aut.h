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

    struct Edge{
        int v, to;
        char w;

        Edge() = default;

        Edge(int v, int to, const char& w) : v(v), to(to), w(w) {}
    };

    static const size_t MAX_VERTEX = 100;
    static const char eps = '_';

    static bool is_eps(char c) {
        return c == eps;
    }

    int start;
    int vertex;
    string alp;
    vector<map<char, int>> edges;
    std::set<int> terms;

    static bool get_bit(int mask, int pos) {
        return (mask & (1 << pos)) != 0;
    }

    vector<Edge> get_edges_list(std::set<int> removed = std::set<int>()) {
        vector<Edge> res;
        for (int v = 0; v < vertex; ++v) {
            for (auto el : edges[v]) {
                int to = el.second;
                if (removed.count(v) || removed.count(to)) continue;
                res.push_back(Edge(v, to, el.first));
            }
        }
        return res;
    }

    void compress_and_assign_edges(const vector<Edge>& pairs) {
        vector<int> xs;
        for (const Edge& edge : pairs) {
            xs.push_back(edge.v);
            xs.push_back(edge.to);
        }
        std::sort(xs.begin(), xs.end());
        xs.erase(std::unique(xs.begin(), xs.end()), xs.end());
        vertex = xs.size();
        std::set<int> new_terms;
        for (auto v : terms) {
            int u = lower_bound(xs.begin(), xs.end(), v) - xs.begin();
            new_terms.insert(u);
        }
        terms = new_terms;
        edges.clear();
        edges.resize(vertex);
        for (const Edge& edge : pairs) {
            int v = lower_bound(xs.begin(), xs.end(), edge.v) - xs.begin();
            int to = lower_bound(xs.begin(), xs.end(), edge.to) - xs.begin();
            char w = edge.w;
            edges[v][w] = to;
        }
        start = lower_bound(xs.begin(), xs.end(), start) - xs.begin();
    }

    void remove_extra_vertices() {
        std::set<int> removed;
        vector<bitset<MAX_VERTEX>> is_reach(vertex, 0);
        for (int v = 0; v < vertex; ++v) {
            is_reach[v][v] = 1;
        }
        for (int i = 0; i < vertex; ++i) {
            for (int v = 0; v < vertex; ++v) {
                for (auto el : edges[v]) {
                    int to = el.second;
                    is_reach[v] |= is_reach[to];
                }
            }
        }
        for (int v = 0; v < vertex; ++v) {
            if (!is_reach[start][v]) {
                removed.insert(v);
            }
            bool ok = 0;
            for (int u = 0; u < vertex; ++u) {
                if (terms.count(u) && is_reach[v][u]) ok = 1;
            }
            if (!ok) removed.insert(v);
        }
        auto list = get_edges_list(removed);
        compress_and_assign_edges(list);
    }


public:
    Automaton() = default;

    Automaton(int vertex, int start, const string& alp, const vector<int>& _terms) 
        : vertex(vertex), start(start), alp(alp) {
            terms = std::set<int> (_terms.begin(), _terms.end());
            edges.resize(vertex);
        }

    int get_vertex() {
        return vertex;
    }

    int get_start() {
        return start;
    }

    int go(int v, char c) {
        
    }

    void add_edge(int v, int to, char w) {
        edges[v][w] = to;
    }

    void remove_eps_edges() {
        vector<bitset<MAX_VERTEX>> is_reach(vertex, 0);
        vector<vector<int>> eps_edges(vertex);
        for (int v = 0; v < vertex; ++v) {
            for (auto el : edges[v]) {
                int to = el.second;
                char w = el.first;
                if (is_eps(w)) {
                    eps_edges[v].push_back(to);
                }
            }
            is_reach[v][v] = 1;
        }

        for (int i = 0; i < vertex; ++i) {
            for (int v = 0; v < vertex; ++v) {
                for (int to : eps_edges[v]) {
                    is_reach[v] |= is_reach[to];
                }
            }
        }

        vector<vector<Edge>> new_edges(vertex);
        for (int v = 0; v < vertex; ++v) {
            for (int u = 0; u < vertex; ++u) {
                if (is_reach[v][u]) {
                    for (auto el : edges[v]) {
                        int to = el.second;
                        char w = el.first;
                        if (!is_eps(w)) {
                            new_edges[v].push_back(Edge(v, to, w));
                        }
                    }
                    if (terms.count(u)) terms.insert(v);
                }
            }
        }
        edges = new_edges;
        remove_extra_vertices();
    }

    bool is_dka() {
        std::set<char> used;
        for (int v = 0; v < vertex; ++v) {
            for (auto el : edges[v]) {
                char w = el.first;
                if (is_eps(w) || used.count(w)) return false;
                used.insert(w);
            }
        }
        return true;
    }

    void to_dka() {
        if (is_dka()) return;
        remove_eps_edges();
        vector<Edge> list;
        std::queue<int> q;
        q.push((1 << start));
        start = (1 << start);
        std::set<int> used;
        std::set<int> new_terms;
        while (!q.empty()) {
            int mask = q.front();
            q.pop();
            if (used.count(mask)) continue;
            for (int v = 0; v < vertex; ++v) {
                if (terms.count(v) && get_bit(mask, v)) {
                    new_terms.insert(mask);
                    break;
                }
            }
            used.insert(mask);
            std::map<char, int> delta;
            for (int v = 0; v < vertex; ++v) {
                if (get_bit(mask, v)) {
                    for (const Edge& edge : edges[v]) {
                        delta[edge.w] |= (1 << edge.to);
                    }
                }
            }
            for (auto el : delta) {
                list.push_back(Edge(mask, el.second, el.first));
                q.push(el.second);
            }
        }
        terms = new_terms;
        compress_and_assign_edges(list);
    }

    void to_pdka() {
        if (!is_dka()) to_dka();
        auto list = get_edges_list();
        int stok = vertex;
        bool ok = 0;
        for (int v = 0; v < vertex; ++v) {
            std::set<char> used;
            for (const Edge& edge : edges[v]) {
                used.insert(edge.w);
            }
            for (char c : alp) {
                if (!used.count(c)) {
                    ok = 1;
                    list.push_back(Edge(v, stok, c));
                }
            }
        }
        if (!ok) return;
        for (char c : alp) {
            list.push_back(Edge(stok, stok, c));
        }
        compress_and_assign_edges(list);
    }

    void to_mpdka() {
        to_pdka();
        vector<int> cl(vertex, 0);
        for (auto v : terms) {
            cl[v] = 1;
        }   
        for (int i = 0; i < vertex; ++i) {
            vector<pair<pair<int, std::map<char, int>>, int>> cmp(vertex);
            for (int v = 0; v < vertex; ++v) {
                cmp[v].first.first = cl[v];
                cmp[v].second = v;
                for (const Edge& edge : edges[v]) {
                    int to = edge.to;
                    char c = edge.w;
                    (cmp[v].first.second)[c] = cl[to];
                }
            }
            std::sort(cmp.begin(), cmp.end());
            cl[cmp[0].second] = 0;
            for (int j = 1; j < vertex; ++j) {
                cl[cmp[j].second] = cl[cmp[j - 1].second];
                if (cmp[j].first != cmp[j - 1].first) cl[cmp[j].second]++;
            }
        }
        start = cl[start];
        std::set<int> new_terms;
        for (int v : terms) {
            new_terms.insert(cl[v]);
        }
        terms = new_terms;
        auto list = get_edges_list();
        vector<Edge> new_list;
        for (auto edge : list) {
            int v = edge.v;
            int to = edge.to;
            char c = edge.w;
            new_list.push_back(Edge(cl[v], cl[to], c));
        }
        std::sort(new_list.begin(), new_list.end());
        new_list.erase(std::unique(new_list.begin(), new_list.end()), new_list.end());
        compress_and_assign_edges(new_list);
    }

    friend bool operator<(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator==(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator<=(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator>=(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator>(const Automaton::Edge& fst, const Automaton::Edge& snd);
    friend bool operator!=(const Automaton::Edge& fst, const Automaton::Edge& snd);

    friend std::ostream& operator<<(std::ostream& out, const Automaton& aut);
    friend std::istream& operator>>(std::istream& in, Automaton& aut);
};

bool operator<(const Automaton::Edge& fst, const Automaton::Edge& snd) {
    pair<pair<int, int>, char> ff = {{fst.v, fst.to}, fst.w};
    pair<pair<int, int>, char> ss = {{snd.v, snd.to}, snd.w};
    return ff < ss;
}

bool operator==(const Automaton::Edge& fst, const Automaton::Edge& snd) {
    pair<pair<int, int>, char> ff = {{fst.v, fst.to}, fst.w};
    pair<pair<int, int>, char> ss = {{snd.v, snd.to}, snd.w};
    return ff == ss;
}

bool operator<=(const Automaton::Edge& fst, const Automaton::Edge& snd) {
    return fst < snd || fst == snd;
}

bool operator>=(const Automaton::Edge& fst, const Automaton::Edge& snd) {
    return snd <= fst;
}

bool operator>(const Automaton::Edge& fst, const Automaton::Edge& snd) {
    return snd < fst;
}

bool operator!=(const Automaton::Edge& fst, const Automaton::Edge& snd) {
    return !(snd == fst);
}

std::istream& operator>>(std::istream& in, Automaton& aut) {
    int n, start;
    string alp;
    in >> n >> start >> alp;
    vector<int> terms;
    int k;
    in >> k;
    for (int i = 0; i < k; ++i) {
        int el;
        in >> el;
        terms.push_back(el);
    }
    aut = Automaton(n, start, alp, terms);
    int m;
    in >> m;
    for (int i = 0; i < m; ++i) {
        int v, to;
        char w;
        in >> v >> to >> w;
        aut.add_edge(v, to, w);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Automaton& aut) {
    out << aut.vertex << ' ' << aut.start << '\n';
    for (int v = 0; v < aut.vertex; ++v) {
        if (aut.terms.count(v)) {
            out << v << ' ';
        }
    }
    out << '\n';
    for (int v = 0; v < aut.vertex; ++v) {
        for (auto edge : aut.edges[v]) {
            out << v << ' ' << edge.to << ' ' << edge.w << '\n';
        }
    }
    return out;
}