#include "automaton.h"
#include "gtest/gtest.h"
#include <fstream>

TEST(MPDKA__TEST, Remove_Eps_Edges_Test) {
    std::ifstream in("txt/mpdka_test.txt");
    Automaton aut;
    in >> aut;
    aut.remove_eps_edges();
    int vertex = aut.get_vertex();
    int start = aut.get_start();
    bool ok = 1;
    for (int v = 0; v < vertex; ++v) {
        vector<int> eps_next = aut.get_all_next(v, '_');
        if (!eps_next.empty()) ok = 0;
    }
    in.close();
    ASSERT_TRUE(ok);
}

TEST(MPDKA__TEST, DKA_Test) {
    std::ifstream in("txt/mpdka_test.txt");
    Automaton aut;
    in >> aut;
    aut.to_dka();
    int vertex = aut.get_vertex();
    int start = aut.get_start();
    string alp = aut.get_alp();
    bool ok = 1;
    for (int v = 0; v < vertex; ++v) {
        for (char c : alp) {
            vector<int> next = aut.get_all_next(v, c);
            if (next.size() > 1) ok = 0;
        }
    }
    in.close();
    ASSERT_TRUE(ok);
}

TEST(MPDKA__TEST, PDKA_Test) {
    std::ifstream in("txt/mpdka_test.txt");
    Automaton aut;
    in >> aut;
    aut.to_pdka();
    int vertex = aut.get_vertex();
    int start = aut.get_start();
    string alp = aut.get_alp();
    bool ok = 1;
    for (int v = 0; v < vertex; ++v) {
        for (char c : alp) {
            vector<int> next = aut.get_all_next(v, c);
            if (next.size() != 1) ok = 0;
        }
    }
    in.close();
    ASSERT_TRUE(ok);
}

TEST(MPDKA__TEST, MPDKA_Test) {
    std::ifstream in("txt/mpdka_test.txt");
    Automaton aut;
    in >> aut;
    string old_alp = aut.get_alp();
    aut.to_mpdka();
    int vertex = aut.get_vertex();
    int start = aut.get_start();
    string alp = aut.get_alp();
    std::ifstream ans_file("txt/mpdka_ans.txt");
    int ans;
    ans_file >> ans;
    in.close();
    ans_file.close();
    ASSERT_EQ(vertex, ans);
    ASSERT_EQ(alp, old_alp);
}