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
  Edge* inverse;

  Edge(int from, int to, int bandwidth, int cost, int roadIndex)
    : from(from)
    , to(to)
    , bandwidth(bandwidth)
    , cost(cost)
    , roadIndex(roadIndex)
    , saturation(0)
    , inverse(NULL)
  {

  }
};

const double infTime = std::numeric_limits<double>::max();

int n; // 2 <= n <= 200, vertice count
int m; // 1 <= m <= 2000, edge count
int k; // 1 <= k <= 100, flow count
// s = 0 - source index, t = n - 1 - sink index
const int maxN = 200;
vector<Edge> edges[maxN];
vector<vector<int> > roads;
pair<int, int> path[maxN];
int totalCost = 0;

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

  fprintf(fo, "%.5f\n", static_cast<double>(totalCost) / k);
  for (int i = 0; i < roads.size(); i++)
  {
    fprintf(fo, "%d ", roads[i].size());
    for (int j = 0; j < roads[i].size(); j++)
    {
      fprintf(fo, "%d ", roads[i][j]);
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

  for (int i = 0; i < n; i++)
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
      int v = edges[u][j].to;
      if (!visited[v]
          && edges[u][j].saturation < edges[u][j].bandwidth)
      {
        if (distance[v] > distance[u] + edges[u][j].cost)
        {
          distance[v] = distance[u] + edges[u][j].cost;
          path[v].first = u;
          path[v].second = j;
        }
      }
    }
  }

  return distance[n - 1] != infWeight;
}

void Backtrace()
{
  roads.push_back(vector<int>());
  vector<int>& road = roads.back();

  int i = n - 1;
  while (path[i].first != -1)
  {
    Edge& e = edges[path[i].first][path[i].second];

    if (!e.inverse)
    {
      Edge e_i = Edge(e.to, e.from, 0, -e.cost, e.roadIndex);
      edges[e.to].push_back(e_i);
      e.inverse = &edges[e.to].back();
    }
    e.saturation += 1;
    e.inverse->saturation -= 1;

    totalCost += e.cost;
    road.push_back(e.roadIndex);

    i = path[i].first;
  }

  std::reverse(road.begin(), road.end());
}
