#include <fstream>
#include <vector>
#include <queue>

using namespace std;

ifstream fin("supercomputer.in");
ofstream fout("supercomputer.out");

vector<vector<int>> adj(100005);  // Adjacency lists
vector<int> in_degree(100005, 0);  // In-degree of nodes
vector<int> set_of_data(100005, 0);  // Dataset for each task

void generateSwitches(vector<vector<int>>& adj, vector<int>& inDegree,
    vector<int>& set_of_data, int required_set, int *switches) {

    queue<int> queues[3];  // Array of queues for tasks

    // Initialize the queues with tasks having zero in-degree
    for (uint i = 1; i <= adj.size(); i++) {
        if (inDegree[i] == 0) {
            queues[set_of_data[i]].push(i);
        }
    }

    // Process tasks and perform necessary context switches until queues are
    // empty
    while (!queues[1].empty() || !queues[2].empty()) {
        int currentTask;

        if (!queues[required_set].empty()) {
            // Dequeue a task from the required set queue
            currentTask = queues[required_set].front();
            queues[required_set].pop();
        } else {
            // Context switch: No task found in the required set queue
            (*switches)++;
            // Switch to the other dataset
            if (required_set == 1) {
                required_set = 2;
            } else {
                required_set = 1;
            }
            continue;
        }

        // Process the current task's adjacent tasks
        for (auto neigh : adj[currentTask]) {
            // If the in-degree becomes zero, enqueue the task to the
            // appropriate dataset queue
            if (inDegree[neigh] == 1) {
                inDegree[neigh]--;
                queues[set_of_data[neigh]].push(neigh);
            } else {
                inDegree[neigh]--;
            }
        }
    }
}

int main() {
    int N, M;
    fin >> N >> M;

    // Read the dataset for each task
    for (int i = 1; i <= N; ++i) {
        fin >> set_of_data[i];
    }

    // Build the adjacency lists and calculate the in-degree of nodes
    for (int i = 0; i < M; ++i) {
        int u, v;
        fin >> u >> v;
        adj[u].push_back(v);
        in_degree[v]++;
    }

    // Make a copy of in_degree
    vector<int> in_degree_copy(in_degree);

    int switches1 = 0;
    int switches2 = 0;
    int required_set1 = 1;
    int required_set2 = 2;

    // Generate switches for 1
    generateSwitches(adj, in_degree, set_of_data,
                required_set1, &switches1);

    // Generate switches for 2
    generateSwitches(adj, in_degree_copy, set_of_data,
                required_set2, &switches2);

    // Output the minimum number of switches
    fout << min(switches1, switches2);

    fin.close();
    fout.close();

    return 0;
}
