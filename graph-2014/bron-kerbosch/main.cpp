#include <cstdlib>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using namespace std;

// int cutoff_bonus = 0;

unordered_map<int, vector<int>> adjacent_vertices;
typedef unordered_set<int> v_set;

// r = c0 U c1
void unite(const std::vector<int>& c0, const v_set& c1, v_set& r)
{
  for (const auto& n : c0)
  {
    if (c1.find(n) != c1.end())
    {
      r.insert(n);
    }
  }
}

// r = a \ b
void set_diff(const v_set& a, const std::vector<int>& b, v_set& r)
{
  r = a;
  for (const auto& i : b)
  {
    r.erase(i);
  }
}

void BronKerbosch(v_set R, v_set P, v_set X)
{
  if (P.empty() && X.empty())
  {
    // result
    printf("clique found: ");
    for (const auto& v : R)
    {
      printf("%d ", v);
    }
    printf("\n");
    return;
  }

  if (P.empty())
  {
    return;
  }

  int pivot = *P.begin();
  v_set Pdn;
  set_diff(P, adjacent_vertices[pivot], Pdn);
  // cutoff_bonus += (P.size() - Pdn.size());
  while (!Pdn.empty())
  {
    int v = *Pdn.begin();
    v_set R1 = R;
    v_set P1;
    v_set X1;

    R1.insert(v);

    unite(adjacent_vertices[v], P, P1);
    unite(adjacent_vertices[v], X, X1);

    BronKerbosch(R1, P1, X1);

    Pdn.erase(v);
    P.erase(v);
    X.insert(v);
  }
}

int main()
{
  FILE* fi = fopen("input.txt", "r");

  int vertexCount = -1;

  auto u_vc = [&](int v)
  {
    if (vertexCount < v)
    {
      vertexCount = v;
    }
  };

  while (true)
  {
    int v0, v1;
    int r = fscanf(fi, "%d -- %d;", &v0, &v1);
    if (feof(fi) || r != 2)
    {
      break;
    }
    adjacent_vertices[v0].push_back(v1);
    adjacent_vertices[v1].push_back(v0);
    u_vc(v0);
    u_vc(v1);
  }

  fclose(fi);

  printf("%d\n", vertexCount);

  v_set pInit;
  for (int i = 0; i <= vertexCount; i++)
  {
    if (adjacent_vertices.find(i) != adjacent_vertices.end())
    {
      pInit.insert(i);
    }
  }

  BronKerbosch(v_set(), pInit, v_set());

  // printf("cutoff bonus: %d\n", cutoff_bonus);
  return 0;
}