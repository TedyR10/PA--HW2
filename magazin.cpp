#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
#include <cstdlib>

using namespace std;

int t = 0;  // Global time variable
vector<int> adj[100005];  // Adjacency lists
vector<int> order;  // Order of nodes in DFS
vector<int> start_time;  // Start time of nodes in DFS
vector<int> end_time;  // End time of nodes in DFS
vector<int> parent;  // Parent of nodes in DFS
vector<int> position;  // Position of nodes in DFS order

// Depth-first search (DFS) function from Lab7
void dfs(int node) {
    ++t;

    // Update the start time of the node
    start_time[node] = t;

    // Add node to DFS order
    order.push_back(node);

    // Set the position of the node in DFS order
    position[node] = order.size() - 1;

    // Process the current node's adjacent nodes
    for (auto neigh : adj[node]) {
        if (!parent[neigh]) {
            parent[neigh] = node;
            dfs(neigh);
        }
    }

    // Update the end time of the node
    end_time[node] = t;
}

int main() {
    ifstream fin("magazin.in");
    ofstream fout("magazin.out");

    int N, Q;
    fin >> N >> Q;

    // Construct the adjacency lists
    for (int i = 2; i <= N; ++i) {
        int x;
        fin >> x;
        adj[x].push_back(i);
    }

    // Initialize the global variables
    order.reserve(N + 1);
    start_time.resize(N + 1, 0);
    end_time.resize(N + 1, 0);
    parent.resize(N + 1, 0);
    position.resize(N + 1, -1);

    dfs(1);  // Perform DFS starting from node 1

    // Process the queries
    for (int i = 0; i < Q; ++i) {
        int d, e;
        fin >> d >> e;

        if (e <= end_time[d] - start_time[d]) {
            fout << order[position[d] + e] << '\n';
        } else {
            fout << "-1\n";
        }
    }

    fin.close();
    fout.close();

    return 0;
}
