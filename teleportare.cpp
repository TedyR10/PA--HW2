#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Edge {
    int to;
    int weight;
    bool teleport;

    Edge(int _to, int _weight, bool _teleport) : to(_to), weight(_weight), teleport(_teleport) {}
};

typedef vector<vector<Edge>> Graph;

vector<int> preprocessTeleportation(const Graph& graph, int N) {
    vector<int> dist(N, INF);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (const Edge& edge : graph[u]) {
            if (!edge.teleport)
                continue;

            int v = edge.to;
            int weight = edge.weight;

            int newDist = dist[u] + weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int dijkstra(const Graph& graph, const vector<int>& teleportDist, int source, int destination) {
    int n = graph.size();
    vector<int> dist(n, INF);
    vector<bool> visited(n, false);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u])
            continue;

        visited[u] = true;

        for (const Edge& edge : graph[u]) {
            int v = edge.to;
            int weight = edge.weight;
            bool teleport = edge.teleport;

            int newDist = dist[u] + weight;
            if (teleport) {
                int period = edge.weight;
                int rem = newDist % period;
                if (rem != 0)
                    newDist += period - rem;

                newDist += teleportDist[v];
            }

            if (newDist < dist[v]) {
                dist[v] = newDist;
                pq.push({dist[v], v});
            }
        }
    }

    return dist[destination];
}

int main() {
    ifstream fin("teleportare.in");
    ofstream fout("teleportare.out");
    int N, M, K;
    fin >> N >> M >> K;

    Graph graph(N);

    for (int i = 0; i < M; i++) {
        int X, Y, T;
        fin >> X >> Y >> T;
        graph[X - 1].emplace_back(Y - 1, T, false);
        graph[Y - 1].emplace_back(X - 1, T, false);
    }

    for (int i = 0; i < K; i++) {
        int X, Y, P;
        fin >> X >> Y >> P;
        graph[X - 1].emplace_back(Y - 1, P, true);
        graph[Y - 1].emplace_back(X - 1, P, true);
    }

    vector<int> teleportDist = preprocessTeleportation(graph, N);

    int minDuration = dijkstra(graph, teleportDist, 0, N - 1);
    fout << minDuration << endl;

    return 0;
}
