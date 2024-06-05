#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/boyer_myrvold_planar_test.hpp>

using namespace boost;

struct Edge {
    int v, flow, C, rev;
};

class Graph {
private:
    int V;
    std::vector<Edge>* adj;
    std::mt19937 gen;
    std::uniform_int_distribution<> capacity_dis;
    std::uniform_int_distribution<> flow_dis;
    void addEdgeToBoostGraph(adjacency_list<vecS, vecS, undirectedS>& bg, int u, int v) {
        add_edge(u, v, bg);
    }
    bool isPlanar(adjacency_list<vecS, vecS, undirectedS>& bg) {
        return boyer_myrvold_planarity_test(bg);
    }

public:
    Graph(int V) : V(V), gen(std::random_device{}()), capacity_dis(1, 10), flow_dis(0, 10) {
        adj = new std::vector<Edge>[V];
    }
    ~Graph() {
        delete[] adj;
    }

    void addEdge(int u, int v) {
        int C = capacity_dis(gen);
        int flow = flow_dis(gen);
        Edge a{ v, flow, C, (int)adj[v].size() };
        Edge b{ u, 0, 0, (int)adj[u].size() };
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool createPlanar(int num_vertices) {
        if (num_vertices < 3) {
            std::cerr << "Graph must have at least 3 vertices." << std::endl;
            return false;
        }
        int side_length = static_cast<int>(std::ceil(std::sqrt(num_vertices)));
        int num_rows = side_length;
        int num_cols = side_length;

        adjacency_list<vecS, vecS, undirectedS> bg(num_vertices);

        for (int row = 0; row < num_rows; ++row) {
            for (int col = 0; col < num_cols; ++col) {
                int v = row * num_cols + col;
                if (v >= num_vertices) break;
                if (col > 0) {
                    int left = v - 1;
                    if (left < num_vertices) {
                        addEdge(v, left);
                        addEdgeToBoostGraph(bg, v, left);
                    }
                }
                if (row > 0) {
                    int up = v - num_cols;
                    if (up < num_vertices) {
                        addEdge(v, up);
                        addEdgeToBoostGraph(bg, v, up);
                    }
                }
            }
        }

        int added_edges = (num_rows - 1) * num_cols + (num_cols - 1) * num_rows;
        added_edges = std::min(added_edges, (3 * num_vertices) - 6);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, num_vertices - 1);

        while (added_edges < (3 * num_vertices - 6)) {
            int u = dis(gen);
            int v = dis(gen);
            if (u != v && !edge(u, v, bg).second) {
                addEdge(u, v);
                addEdgeToBoostGraph(bg, u, v);
                if (isPlanar(bg)) {
                    added_edges++;
                }
                else {
                    remove_edge(u, v, bg);
                }
            }
        }

        return isPlanar(bg);
    }

    void printGraph() {
        for (int u = 0; u < V; ++u) {
            std::cout << "Vertex " << u << ":\n";
            for (const auto& e : adj[u]) {
                std::cout << " -> " << e.v << " (flow: " << e.flow << ", C: " << e.C << ", rev: " << e.rev << ")\n";
            }
        }
    }
};

int main() {
    int num_vertices = 100;
    Graph g(num_vertices);

    if (g.createPlanar(num_vertices)) {
        std::cout << "The graph is planar." << std::endl;
        //g.printGraph();
    }
    else {
        std::cout << "Failed to create planar graph." << std::endl;
    }

    return 0;
}
