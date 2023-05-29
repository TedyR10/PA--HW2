#include <fstream>
#include <vector>
#include <stack>

using namespace std;

// Adjacency lists & directed acyclic graph
vector<vector<int>> adj, adj_transpose, dag;
stack<int> s;  // Stack for topological sorting
vector<int> visited;  // Visited array for DFS
vector<int> component;  // Component IDs for nodes
vector<int> in_degree;  // In-degree of components in DAG

// Depth-first search (DFS) function
void dfs(int node, int component_id, int type) {
    if (type == 1) {
        visited[node] = 1;
        for (auto neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor, component_id, type);
            }
        }
        s.push(node);  // Push nodes to stack after visiting all neighbors
    } else {
        visited[node] = 1;
        component[node] = component_id;
        for (auto neighbor : adj_transpose[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor, component_id, type);
            }
        }
    }
}

// Function to calculate the number of rails
void getRails(int N, int S, int& rails) {
    int scc = 0;

    // First DFS traversal for topological sorting
    for (int i = 1; i <= N; i++) {
        if (!visited[i]) {
            dfs(i, 0, 1);
        }
    }

    fill(visited.begin(), visited.end(), 0);

    // Second DFS traversal for scc identification
    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            dfs(node, scc++, 2);
        }
    }

    dag.resize(scc);
    in_degree.resize(scc, 0);

    // Construct the Directed Acyclic Graph (DAG) from scc
    for (int i = 1; i <= N; i++) {
        for (uint j = 0; j < adj[i].size(); j++) {
            int neigh = adj[i][j];
            if (component[i] != component[neigh]) {
                dag[component[i]].push_back(component[neigh]);
            }
        }
    }

    // Calculate the in-degree of scc in DAG
    for (int i = 0; i < scc; i++) {
        for (uint j = 0; j < dag[i].size(); j++) {
            int neigh = dag[i][j];
            in_degree[neigh]++;
        }
    }

    rails = 0;

    // Count the number of scc with in-degree zero
    for (int i = 0; i < scc; i++) {
        if (!in_degree[i] && i != component[S]) {
            rails++;
        }
    }
}

int main() {
    int N, M, S, rails;

    ifstream fin("ferate.in");
    ofstream fout("ferate.out");

    fin >> N >> M >> S;

    // Initialize the global variables
    adj.resize(N + 1);
    adj_transpose.resize(N + 1);
    visited.resize(N + 1, 0);
    component.resize(N + 1);

    // Read edges and construct the adjacency lists
    for (int i = 0; i < M; i++) {
        int u, v;
        fin >> u >> v;
        adj[u].push_back(v);
        adj_transpose[v].push_back(u);
    }

    // Calculate the number of rails needed
    getRails(N, S, rails);

    fout << rails << '\n';

    fin.close();
    fout.close();

    return 0;
}
