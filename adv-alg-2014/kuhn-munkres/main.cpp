#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

const int maxN = 12;
const int maxK = 12;

int n;
// see K in problem desc
int m;
char targetStr[maxK + 1];
char faces[maxN][7];

// g[i][j] == k means there is an edge from i to k
int g[maxK][maxN];

// size of g[i]
int gEdgeCount[maxK];

bool visited[maxK];
int matching[maxK] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
int directMatching[maxK] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

bool dfsKuhn(int v)
{
  if (visited[v])
  {
    return false;
  }

  visited[v] = true;

  for (int i = 0; i < gEdgeCount[v]; i++)
  {
    int adj = g[v][i];
    if (matching[adj] == -1
        || dfsKuhn(matching[adj]))
    {
      matching[adj] = v;
      directMatching[v] = adj;
      visited[v] = false;
      return true;
    }
  }

  visited[v] = false;
}

int main()
{
  FILE* fi = fopen("input.txt", "r");
  fscanf(fi, "%d", &n);
  fscanf(fi, "%s", targetStr);
  m = strlen(targetStr);
  for (int i = 0; i < n; i++)
  {
    fscanf(fi, "%s", faces[i]);
  }
  fclose(fi);

  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < m; j++)
    {
      for (int k = 0; k < strlen(faces[i]); k++)
      {
        if (targetStr[j] == faces[i][k])
        {
          bool present = false;
          for (int l = 0; l < gEdgeCount[j]; l++)
          {
            if (g[j][l] == i)
            {
              present = true;
            }
          }
          if (!present)
          {
            g[j][gEdgeCount[j]] = i;
            gEdgeCount[j]++;
          }
        }
      }
    }
  }

  for (int i = 0; i < m; i++)
  {
    dfsKuhn(i);
  }

  FILE* fo = fopen("output.txt", "w");

  for (int i = 0; i < m; i++)
  {
    if (directMatching[i] == -1)
    {
      fprintf(fo, "0");
      fclose(fo);
      return 0;
    }
  }

  for (int i = 0; i < m; i++)
  {
    fprintf(fo, "%d ", directMatching[i] + 1);
  }

  fclose(fo);
  return 0;
}

