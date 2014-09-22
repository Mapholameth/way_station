#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge
{
  int from;
  int to;
  int bandwidth;
  int cost;
  int saturation;
  int roadIndex; // refers to road index from input file
  int inverse;

  Edge(int from, int to, int bandwidth, int cost, int roadIndex)
    : from(from)
    , to(to)
    , bandwidth(bandwidth)
    , cost(cost)
    , roadIndex(roadIndex)
    , saturation(0)
    , inverse(-1)
  {

  }
};

const double infTime = std::numeric_limits<double>::max();

int n; // 2 <= n <= 200, vertice count
int m; // 1 <= m <= 2000, edge count
int k; // 1 <= k <= 100, flow count
// s = 0 - source index, t = n - 1 - sink index
const int maxN = 201;
vector<Edge> edges[maxN];

int potentials[maxN];
pair<int, int> path[maxN];

bool DijkstraSearch();
void Backtrace();

int main()
{
  FILE* fi = fopen("brides.in", "r");
  fscanf(fi, "%d%d%d", &n, &m, &k);
  for (int i = 0; i < m; i++)
  {
    int v0;
    int v1;
    int time;
    fscanf(fi, "%d%d%d", &v0, &v1, &time);
    v0--;
    v1--;
    edges[v0].push_back(Edge(v0, v1, 1, time, i + 1));
    edges[v1].push_back(Edge(v1, v0, 1, time, i + 1));

    edges[v0].back().inverse = edges[v1].size() - 1;
    edges[v1].back().inverse = edges[v0].size() - 1;
  }
  fclose(fi);

  FILE* fo = fopen("brides.out", "w");

  for (int i = 0; i < k; i++)
  {
    if (!DijkstraSearch())
    {
      fprintf(fo, "-1");
      fclose(fo);
      return 0;
    }

    Backtrace();
  }

  double totalCost = 0.0;
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < edges[i].size(); j++)
    {
      Edge& e = edges[i][j];
      if (e.saturation > 0)
      {
        totalCost += e.cost;
      }
    }
  }

  fprintf(fo, "%.5f\n", static_cast<double>(totalCost) / k);

  for (int i = 0; i < k; i++)
  {
    int v = 0;
    vector<int> road;
    while (v != n - 1)
    {
      for (int j = 0; j < edges[v].size(); j++)
      {
        Edge& e = edges[v][j];
        if (e.saturation > 0)
        {
          e.saturation = 0;
          v = e.to;
          road.push_back(e.roadIndex);
        }
      }
    }
    fprintf(fo, "%d ", road.size());
    for (int i = 0; i < road.size(); i++)
    {
      fprintf(fo, "%d ", road[i]);
    }
    fprintf(fo, "\n");
  }

  fclose(fo);
  return 0;
}

bool DijkstraSearch()
{
  const int infWeight = std::numeric_limits<int>::max();
  static bool visited[maxN];
  static int distance[maxN];
  fill(visited, visited + maxN, false);
  fill(distance, distance + maxN, infWeight);
  fill(path, path + maxN, make_pair(-1, -1));
  distance[0] = 0;

  while (true)
  {
    int u = -1;
    int minWeight = infWeight;
    for (int j = 0; j < n; j++)
    {
      if (!visited[j]
          && distance[j] < minWeight)
      {
        minWeight = distance[j];
        u = j;
      }
    }

    if (u == -1)
    {
      break;
    }

    visited[u] = true;
    for (int j = 0; j < edges[u].size(); j++)
    {
      Edge& e = edges[u][j];
      int v = e.to;
      if (!visited[v]
          && e.saturation < e.bandwidth)
      {
        int newWeight = distance[u] + potentials[u] - potentials[v];
        newWeight += e.saturation < 0 ? -e.cost : e.cost;
        if (distance[v] > newWeight)
        {
          distance[v] = newWeight;
          path[v].first = u;
          path[v].second = j;
        }
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    potentials[i] = min(potentials[i] + distance[i], infWeight);
  }

  return distance[n - 1] != infWeight;
}

void Backtrace()
{
  int i = n - 1;
  while (path[i].first != -1)
  {
    Edge& e = edges[path[i].first][path[i].second];
    Edge& e_i = edges[e.to][e.inverse];

    e.saturation += 1;
    e_i.saturation -= 1;

    i = path[i].first;
  }
}
