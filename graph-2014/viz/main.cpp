#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <limits>

using namespace std;

struct point
{
  string name;
  double x;
  double y;
  point(double x, double y) : x(x), y(y) {}
};

struct edge
{
  string v0;
  string v1;
  string label;
};

int main()
{
  FILE* fileNodes = fopen("test.nodes", "r");
  vector<point> nodes;

  double xMin = numeric_limits<double>::max();
  double xMax = numeric_limits<double>::min();
  double yMin = numeric_limits<double>::max();
  double yMax = numeric_limits<double>::min();

  while (true)
  {
    char name[256];
    point p(0.0, 0.0);
    fscanf(fileNodes, "%s %lf %lf", &name, &p.y, &p.x);
    if (feof(fileNodes)) break;
    p.name = string(name);
    // printf("%s %lf %lf\n", p.name.c_str(), p.x, p.y);
    nodes.push_back(p);
    if (p.x < xMin) xMin = p.x;
    if (p.x > xMax) xMax = p.x;
    if (p.y < yMin) yMin = p.y;
    if (p.y > yMax) yMax = p.y;
  }

  double xScale = 400.0f / (xMax - xMin);
  double yScale = 400.0f / (yMax - yMin);

  for (auto& p : nodes)
  {
    p.x -= xMin;
    p.y -= yMin;
    p.x *= xScale;
    p.y *= yScale;
  }

  fclose(fileNodes);

  FILE* fileEdges = fopen("test.eds", "r");
  vector<edge> edges;
  while (true)
  {
    char t[256];
    edge e;
    fscanf(fileNodes, "%s", &t);
    if (feof(fileNodes)) break;
    e.v0 = string(t);
    fscanf(fileNodes, "%s", &t);
    if (feof(fileNodes)) break;
    e.v1 = string(t);
    fscanf(fileNodes, "%s", &t);
    if (feof(fileNodes)) break;
    e.label = string(t);

    edges.push_back(e);
  }

  fclose(fileEdges);

  FILE* fOut = fopen("out.gv", "w");

  fprintf(fOut, R"(
graph "complete graph" {
  node [shape=circle, fontname=Consolas, height=0.01, width=0.01];
)");

  for (const auto& p : nodes)
    fprintf(fOut, "  %s [pos=\"%lf,%lf!\",label=\"\"];\n", p.name.c_str(), p.x, p.y);
  for (const auto& e : edges)
    fprintf(fOut, "  %s -- %s;\n", e.v0.c_str(), e.v1.c_str());

  fprintf(fOut, "}");

  fclose(fOut);
}

