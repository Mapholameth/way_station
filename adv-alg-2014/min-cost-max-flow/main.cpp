#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>

using namespace std;

const int maxN = 1000;
const int infWeight = std::numeric_limits<int>::max();
bool visited[maxN];
int distance[maxN];

int n;
int m;
int s;
int edges[maxN][maxN];

int main()
{
  memset(edges, infWeight, maxN * maxN);

  FILE* fi = fopen("input.txt", "r");
  fscanf(fi, "%d%d%d", &n, &m, &s);
  s--;
  for (int i = 0; i < m; i++)
  {
    int u;
    int v;
    int w;
    fscanf(fi, "%d%d%d", &u, &v, &w);
    edges[u - 1][v - 1] = w;
  }
  fclose(fi);

  for (int i = 0; i < n; i++)
  {
    edges[i][i] = 0;
    distance[i] = edges[s][i];
  }

  for (int i = 0; i < n; i++)
  {
    int v = -1;
    int minWeight = infWeight;
    for (int j = 0; j < n; j++)
    {
      if (!visited[j]
          && minWeight > distance[j])
      {
        minWeight = distance[j];
        v = j;
      }
    }
    if (v == -1)
    {
      break;
    }

    visited[v] = true;
    for (int u = 0; u < n; u++)
    {
      if (!visited[u]
          && edges[v][u] != -1)
      {
        if (distance[u] > distance[v] + edges[v][u])
        {
          distance[u] = distance[v] + edges[v][u];
        }
      }
    }
  }

  FILE* fo = fopen("output.txt", "w");
  for (int i = 0; i < n; i++)
  {
    if (distance[i] == infWeight)
    {
      fprintf(fo, "-1 ");
    }
    else
    {
      fprintf(fo, "%d ", distance[i]);
    }
  }
  fclose(fo);

  return 0;
}
