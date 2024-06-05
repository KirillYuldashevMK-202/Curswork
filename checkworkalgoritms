#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <algorithm>

struct Edge {
    int v;
    int flow;
    int C;
    int rev;
    Edge(int _v, int _flow, int _C, int _rev) : v(_v), flow(_flow), C(_C), rev(_rev) {}
};

class Graph {
private:
    int V;
    std::vector<std::vector<Edge>> adj;

public:
    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int u, int v, int C) {
        adj[u].emplace_back(v, 0, C, adj[v].size());
        adj[v].emplace_back(u, 0, 0, adj[u].size() - 1);
    }

    bool DFS_Berge(int u, int t, std::vector<int>& parent) {
        std::fill(parent.begin(), parent.end(), -1);
        std::stack<int> stack;
        stack.push(u);
        parent[u] = -2;

        while (!stack.empty()) {
            int v = stack.top();
            stack.pop();

            for (Edge& e : adj[v]) {
                if (parent[e.v] == -1 && e.flow < e.C) {
                    parent[e.v] = v;
                    if (e.v == t)
                        return true;
                    stack.push(e.v);
                }
            }
        }
        return false;
    }

    int BergeAlgorithm(int s, int t) {
        int max_flow = 0;
        std::vector<int> parent(V);

        while (true) {
            if (!DFS_Berge(s, t, parent))
                break;

            int path_flow = INT_MAX;

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (Edge& e : adj[u]) {
                    if (e.v == v) {
                        path_flow = std::min(path_flow, e.C - e.flow);
                        break;
                    }
                }
            }

            for (int v = t; v != s; v = parent[v]) {
                int u = parent[v];
                for (Edge& e : adj[u]) {
                    if (e.v == v) {
                        e.flow += path_flow;
                        adj[v][e.rev].flow -= path_flow;
                        break;
                    }
                }
            }

            max_flow += path_flow;
        }

        return max_flow;
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
};

int main() {
    /*Graph graph1(7);
    Graph graph2(7);

    graph1.addEdge(0, 1, 16);
    graph1.addEdge(0, 2, 13);
    graph1.addEdge(1, 2, 11);
    graph1.addEdge(1, 3, 12);
    graph1.addEdge(2, 1, 4);
    graph1.addEdge(2, 4, 14);
    graph1.addEdge(3, 2, 9);
    graph1.addEdge(3, 5, 20);
    graph1.addEdge(4, 3, 7);
    graph1.addEdge(4, 5, 4);
    graph1.addEdge(5, 6, 4);
    graph1.addEdge(6, 0, 7);

    graph2.addEdge(0, 1, 16);
    graph2.addEdge(0, 2, 13);
    graph2.addEdge(1, 2, 11);
    graph2.addEdge(1, 3, 12);
    graph2.addEdge(2, 1, 4);
    graph2.addEdge(2, 4, 14);
    graph2.addEdge(3, 2, 9);
    graph2.addEdge(3, 5, 20);
    graph2.addEdge(4, 3, 7);
    graph2.addEdge(4, 5, 4);
    graph2.addEdge(5, 6, 4);
    graph2.addEdge(6, 0, 7);

    int s = 0, t = 6;*/

    Graph graph1(15);
    Graph graph2(15);

    graph1.addEdge(0, 1, 10);
    graph1.addEdge(0, 2, 5);
    graph1.addEdge(0, 3, 15);
    graph1.addEdge(1, 4, 10);
    graph1.addEdge(1, 5, 15);
    graph1.addEdge(2, 6, 10);
    graph1.addEdge(3, 7, 10);
    graph1.addEdge(4, 8, 10);
    graph1.addEdge(5, 9, 15);
    graph1.addEdge(6, 10, 10);
    graph1.addEdge(7, 11, 10);
    graph1.addEdge(8, 12, 10);
    graph1.addEdge(9, 13, 15);
    graph1.addEdge(10, 14, 10);
    graph1.addEdge(11, 14, 10);
    graph1.addEdge(12, 14, 10);
    graph1.addEdge(13, 14, 15);

    graph2.addEdge(0, 1, 10);
    graph2.addEdge(0, 2, 5);
    graph2.addEdge(0, 3, 15);
    graph2.addEdge(1, 4, 10);
    graph2.addEdge(1, 5, 15);
    graph2.addEdge(2, 6, 10);
    graph2.addEdge(3, 7, 10);
    graph2.addEdge(4, 8, 10);
    graph2.addEdge(5, 9, 15);
    graph2.addEdge(6, 10, 10);
    graph2.addEdge(7, 11, 10);
    graph2.addEdge(8, 12, 10);
    graph2.addEdge(9, 13, 15);
    graph2.addEdge(10, 14, 10);
    graph2.addEdge(11, 14, 10);
    graph2.addEdge(12, 14, 10);
    graph2.addEdge(13, 14, 15);

    int s = 0, t = 13;

    int max_flow1 = graph1.EdmondsKarp(s, t);
    int max_flow2 = graph2.BergeAlgorithm(s, t);

    std::cout << "Max flow using Berge Algorithm for graph 1: " << max_flow1 << std::endl;
    std::cout << "Max flow using Edmonds-Karp Algorithm for graph 2: " << max_flow2 << std::endl;

    return 0;
}
