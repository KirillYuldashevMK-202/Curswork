#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <algorithm>
#include <queue>
#include <chrono>
#include <stack>
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
        if (C == 0) return;
        int flow = flow_dis(gen);
        Edge a{ v, flow, C, (int)adj[v].size() };
        Edge b{ u, 0, 0, (int)adj[u].size() };
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    bool DFS_Berge(int u, int t, std::vector<int>& parent) {
        std::fill(parent.begin(), parent.end(), -1);
        std::queue<int> queue;
        queue.push(u);
        parent[u] = -2;
        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();
            for (Edge& e : adj[v]) {
                if (parent[e.v] == -1 && e.flow < e.C) {
                    parent[e.v] = v;
                    if (e.v == t)
                        return true;
                    queue.push(e.v);
                }
            }
        }
        return false;
    }


    int BergeAlgorithm(int s, int t) {
        int max_flow = 0;
        std::vector<int> parent(V);
        while (true) {
            if (!DFS_Berge(s, t, parent)) {
                break;
            }
            int path_flow = INT_MAX;
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (const Edge& e : adj[u]) {
                    if (e.v == v) {
                        path_flow = std::min(path_flow, e.C - e.flow);
                        break;
                    }
                }
            }
            if (path_flow == 0) {
                break;
            }
            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (Edge& e : adj[u]) {
                    if (e.v == v) {
                        e.flow += path_flow;
                        for (Edge& rev : adj[v]) {
                            if (rev.v == u && rev.rev == (&e - &adj[u][0])) {
                                rev.flow -= path_flow;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
            max_flow += path_flow;
        }
        return max_flow;
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

    void createRandom(int num_edges) {
        for (int u = 0; u < V; ++u) {
            for (int v = u + 1; v < V; ++v) {
                if (std::uniform_int_distribution<>(0, 1)(gen)) {
                    addEdge(u, v);
                }
            }
        }
    }

    void printGraph() {
        for (int u = 0; u < V; ++u) {
            std::cout << "Vertex " << u << ":\n";
            for (const auto& e : adj[u]) {
                if (e.C > 0) {
                    std::cout << " -> " << e.v << " (flow: " << e.flow << ", C: " << e.C << ", rev: " << e.rev << ")\n";
                }
            }
        }
    }

    bool BFS(int s, int t, std::vector<int>& parent) {
        fill(parent.begin(), parent.end(), -1);
        parent[s] = -2;
        std::queue<std::pair<int, int>> q;
        q.push({ s, INT_MAX });

        while (!q.empty()) {
            int u = q.front().first;
            int flow = q.front().second;
            q.pop();

            for (const Edge& e : adj[u]) {
                int v = e.v;
                if (parent[v] == -1 && e.flow < e.C) {
                    parent[v] = u;
                    int new_flow = std::min(flow, e.C - e.flow);
                    if (v == t)
                        return new_flow;
                    q.push({ v, new_flow });
                }
            }
        }

        return false;
    }

    int EdmondsKarp(int s, int t) {
        int max_flow = 0;
        std::vector<int> parent(V);

        while (true) {
            int path_flow = BFS(s, t, parent);
            if (path_flow == 0)
                break;
            max_flow += path_flow;

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (Edge& e : adj[u]) {
                    if (e.C == 0) {
                        continue;
                    }
                    if (e.v == v) {
                        e.flow += path_flow;
                        adj[v][e.rev].flow -= path_flow;
                        break;
                    }
                }
            }
        }

        return max_flow;
    }
    Graph(const Graph& other) {
        V = other.V;
        adj = new std::vector<Edge>[V];
        for (int i = 0; i < V; ++i) {
            adj[i] = other.adj[i];
        }
    }

    Graph& operator=(const Graph& other) {
        if (this != &other) {
            delete[] adj; 
            V = other.V;
            adj = new std::vector<Edge>[V];
            for (int i = 0; i < V; ++i) {
                adj[i] = other.adj[i];
            }
        }
        return *this;
    }
};

int main() {
    int num_vertices = 50;

    //Graph g1(num_vertices);
    //g1.createPlanar(num_vertices);

    ////g1.printGraph(); 

    //auto start1 = std::chrono::high_resolution_clock::now();
    //int max_flow1 = g1.EdmondsKarp(0, num_vertices - 1);
    //auto end1 = std::chrono::high_resolution_clock::now();
    //auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
    //std::cout << "Average max flow time: " << duration1 / 1000.0 << " ms" << std::endl;


    //Graph g(num_vertices);
    //g.createPlanar(num_vertices);

    ////g.printGraph(); 

    //auto start = std::chrono::high_resolution_clock::now();
    //int max_flow = g.BergeAlgorithm(0, num_vertices-1);
    //auto end = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    //std::cout << "Average max flow time: " << duration / 1000.0 << " ms" << std::endl;

    Graph origin(num_vertices);
    //origin.createRandom(num_vertices);
    origin.createPlanar(num_vertices);

    Graph g1 = origin;
    Graph g = origin;

    //g.printGraph(); 

    auto start1 = std::chrono::high_resolution_clock::now();
    int max_flow1 = g1.EdmondsKarp(0, num_vertices - 1);
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1).count();
    std::cout << "Average max flow time: " << duration1 / 1000.0 << " ms" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    int max_flow = g.BergeAlgorithm(0, num_vertices - 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Average max flow time: " << duration / 1000.0 << " ms" << std::endl;

    //g1.printGraph();

    return 0;
}

