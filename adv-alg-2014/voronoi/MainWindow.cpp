#include "MainWindow.hpp"
#include "ui_mainwindow.h"

#include <vector>
#include <algorithm>
#include <cassert>

#include <QPainter>
#include <QMouseEvent>

using namespace std;

struct Matrix2
{
  float e11;
  float e12;
  float e21;
  float e22;

  Matrix2(float e11, float e12
          , float e21, float e22)
    : e11(e11), e12(e12)
    , e21(e21), e22(e22)
  {
  }

  float Det() const
  {
    return e11 * e22 - e12 * e21;
  }
};

struct Vector2
{
  float x;
  float y;

  explicit Vector2(float x, float y) : x(x), y(y) {}

  float operator[](int i)
  {
    assert(i >= 0 && i <= 1);
    return *(&x + i);
  }

  Vector2 operator +(const Vector2 &rhs) const
  {
    return Vector2(x + rhs.x, y + rhs.y);
  }

  Vector2 operator -(const Vector2 &rhs) const
  {
    return Vector2(x - rhs.x, y - rhs.y);
  }

  Vector2 operator *(float rhs) const
  {
    return Vector2(x * rhs, y * rhs);
  }

  Vector2 operator /(float rhs) const
  {
    assert(rhs != 0.0f);
    float t = 1.0f / rhs;
    return Vector2(x * t, y * t);
  }

  friend Vector2 operator *(float lhs, const Vector2 &rhs)
  {
    return Vector2(rhs.x * lhs, rhs.y * lhs);
  }

  Vector2& operator +=(const Vector2 &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  Vector2& operator -=(const Vector2 &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    return *this;
  }

  Vector2& operator *=(float rhs)
  {
    x *= rhs;
    y *= rhs;
    return *this;
  }

  Vector2& operator /=(float rhs)
  {
    return *this = *this / rhs;
  }

  Vector2 operator -() const
  {
    return Vector2(-x, -y);
  }

  float operator *(const Vector2 &rhs) const
  {
    return x * rhs.x + y * rhs.y;
  }

  bool operator ==(const Vector2 &rhs) const
  {
    return fabsf(x - rhs.x) < 0.001f
        && fabsf(y - rhs.y) < 0.001f;
  }

  bool operator !=(const Vector2 &rhs) const
  {
    return !(*this == rhs);
  }

  float Length() const
  {
    return sqrtf(x * x + y * y);
  }

  Vector2 GetPerpendicular() const
  {
    return Vector2(-y, x);
  }

  Vector2 Normalized() const
  {
    return *this / Length();
  }

  float Normalize()
  {
    float l = Length();
    assert(l != 0.0f);
    *this /= l;
    return l;
  }

  Vector2 ComponentwiseMultiply(const Vector2& rhs)
  {
    Vector2 r = *this;
    r.x *= rhs.x;
    r.y *= rhs.y;
    return r;
  }
};

float HalfPlaneSign(const Vector2 &u0, const Vector2 &u1, const Vector2 &p)
{
  return (u0.x - p.x) * (u1.y - p.y)
       - (u0.y - p.y) * (u1.x - p.x);
}

bool IntersectLines(const Vector2 &u0, const Vector2 &u1
                  , const Vector2 &v0, const Vector2 &v1
                  , Vector2 &Result)
{
  float a1 = u1.y - u0.y;
  float b1 = u0.x - u1.x;
  float a2 = v1.y - v0.y;
  float b2 = v0.x - v1.x;
  Matrix2 deltaMatrix(a1, b1, a2, b2);
  float deltaDet = deltaMatrix.Det();

  // Прямые параллельны, т.е. a1b2 - a2b1 == 0;
  // Кстати, условие перпендикулярности: a1a2 == -b1b2;
  if (fabsf(deltaDet) < 0.0001f)
  {
    return false;
  }

  // @todo А что, если прямые совпадают?
  float c1 = u1.y * u0.x - u1.x * u0.y;    //a1 * u0.x + b1 * u0.y;
  float c2 = v1.y * v0.x - v1.x * v0.y;    //a2 * v0.x + b2 * v0.y;
  Result = Vector2(Matrix2(c1, b1, c2, b2).Det() / deltaDet,
                   Matrix2(a1, c1, a2, c2).Det() / deltaDet);
  return true;
}

bool IntersectSegments(const Vector2 &u0, const Vector2 &u1
                     , const Vector2 &v0, const Vector2 &v1
                     , Vector2 &Result)
{
  if (HalfPlaneSign(u0, u1, v0) * HalfPlaneSign(u0, u1, v1) > 0)
  {
    return false;
  }

  if (HalfPlaneSign(v0, v1, u0) * HalfPlaneSign(v0, v1, u1) > 0)
  {
    return false;
  }

  // In the "IntersectLines" lies check if lines are parallel
  // but at that point we're already know that they're not
  return IntersectLines(u0, u1, v0, v1, Result);
}

bool AreSegmentsIntersect(const Vector2 &u0, const Vector2 &u1
                        , const Vector2 &v0, const Vector2 &v1)
{
  if (HalfPlaneSign(u0, u1, v0) * HalfPlaneSign(u0, u1, v1) > 0)
  {
    return false;
  }

  if (HalfPlaneSign(v0, v1, u0) * HalfPlaneSign(v0, v1, u1) > 0)
  {
    return false;
  }

  return true;
}

void CalcConvexHull(std::vector<Vector2> &a)
{
    class CalcConvexHullHelper
    {
    public:
        static bool cmp (Vector2 a, Vector2 b)
        {
            return a.x < b.x || a.x == b.x && a.y < b.y;
        }

        static bool cw (Vector2 a, Vector2 b, Vector2 c)
        {
            return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) < 0;
        }

        static bool ccw (Vector2 a, Vector2 b, Vector2 c)
        {
            return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y) > 0;
        }
    };

    if (a.size() <= 1)
    {
      return;
    }
    sort (a.begin(), a.end(), &CalcConvexHullHelper::cmp);
    Vector2 p1 = a[0],  p2 = a.back();
    std::vector<Vector2> up, down;
    up.push_back (p1);
    down.push_back (p1);
    for (size_t i=1; i<a.size(); ++i)
    {
        if (i==a.size()-1 || CalcConvexHullHelper::cw (p1, a[i], p2))
        {
            while (up.size()>=2 && !CalcConvexHullHelper::cw (up[up.size()-2], up[up.size()-1], a[i]))
                up.pop_back();
            up.push_back (a[i]);
        }
        if (i==a.size()-1 || CalcConvexHullHelper::ccw (p1, a[i], p2))
        {
            while (down.size()>=2 && !CalcConvexHullHelper::ccw (down[down.size()-2], down[down.size()-1], a[i]))
                down.pop_back();
            down.push_back (a[i]);
        }
    }
    a.clear();
    for (size_t i=0; i<up.size(); ++i)
        a.push_back (up[i]);
    for (size_t i=down.size()-2; i>0; --i)
        a.push_back (down[i]);
}

struct Edge
{
  Vector2 p0;
  Vector2 p1;

  Edge(Vector2 p0, Vector2 p1)
    : p0(p0)
    , p1(p1)
  {

  }
};

struct Triangle
{
  int v0 = -1;
  int v1 = -1;
  int v2 = -1;
  Triangle* adj0 = nullptr;
  Triangle* adj1 = nullptr;
  Triangle* adj2 = nullptr;

  int FindLowestVertex()
  {
    return -1;
  }
};

vector<Vector2> points;
vector<Vector2> gIsectPoints;
vector<Edge> edges;
vector<Triangle> triangles;
int areaWidth = 800;
int areaHeight = 600;
int margin = 50;

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  for (int i = 0; i < 100; i++)
  {
    break;
    points.push_back(Vector2(margin + rand() % (areaWidth - margin * 2)
                           , margin + rand() % (areaHeight - margin * 2)));
  }

  points.push_back(Vector2(areaWidth * 0.5, areaHeight * -1));
  points.push_back(Vector2(areaWidth * 0.5, areaHeight * 2));
  points.push_back(Vector2(areaWidth * -1.0f, areaHeight * 0.5));
  points.push_back(Vector2(areaWidth * 2.0f, areaHeight * 0.5));
  points.push_back(Vector2(areaWidth * 0.5f, areaHeight * 0.5));

  RecalculateStuff();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
  QPainter painter(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::TextAntialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform);

  QBrush brush;
  QPen pen;
  pen.setColor(QColor(0, 0, 0, 128));
  pen.setWidth(2);
  painter.setPen(pen);
  for (auto& e : edges)
  {
    painter.drawLine(e.p0.x, e.p0.y, e.p1.x, e.p1.y);
  }

  for (auto& t : triangles)
  {
    painter.drawLine(points[t.v0].x, points[t.v0].y, points[t.v1].x, points[t.v1].y);
    painter.drawLine(points[t.v1].x, points[t.v1].y, points[t.v2].x, points[t.v2].y);
    painter.drawLine(points[t.v2].x, points[t.v2].y, points[t.v0].x, points[t.v0].y);
    painter.drawText(points[t.v0].x, points[t.v0].y, "0");
    painter.drawText(points[t.v1].x, points[t.v1].y, "1");
    painter.drawText(points[t.v2].x, points[t.v2].y, "2");
  }

  pen.setWidth(1);
  pen.setColor(Qt::black);
  brush.setColor(QColor(255, 128, 75, 255));
  brush.setStyle(Qt::SolidPattern);
  painter.setPen(pen);
  painter.setBrush(brush);

  for (auto& p : points)
  {
    painter.drawEllipse(QPointF(p.x, p.y), 3, 3);
  }

  brush.setColor(Qt::green);
  painter.setBrush(brush);
  for (auto& p : gIsectPoints)
  {
    painter.drawEllipse(QPointF(p.x, p.y), 2, 2);
  }
}

void MainWindow::mousePressEvent(QMouseEvent* e)
{
  if (e->button() == Qt::LeftButton)
  {
    points.push_back(Vector2(e->x(), e->y()));
  }
  else if (e->button() == Qt::RightButton)
  {
    points.erase(std::remove_if(points.begin(), points.end(), [=](Vector2 p)
    {
      return sqrt(pow((p.x - e->x()), 2) + pow((p.y - e->y()), 2)) < 10;
    }), points.end());
  }
  this->RecalculateStuff();
  this->repaint();
}

void MainWindow::RecalculateStuff()
{
  std::sort(points.begin(), points.end(), [](Vector2 a, Vector2 b)
  {
    return a.x < b.x;
//    return atan2(a.y, a.x) < atan2(b.y, b.x);
  });

  edges.clear();
  triangles.clear();

  //Split(0, points.size());
  BruteForceVoronoi();
}

// [l; r)
void MainWindow::Split(int l, int r)
{
  if (r - l <= 3)
  {
    Triangle t;
    if (r - l == 3)
    {
      int i0 = l;
      int i1 = l + 1;
      int i2 = l + 2;
      int* pi0 = &i0;
      int* pi1 = &i1;
      int* pi2 = &i2;

      if (points[*pi0].y > points[i1].y)
      {
        pi0 = &i1;
        pi1 = &i0;
      }
      if (points[*pi0].y > points[i2].y)
      {
        pi2 = pi0;
        pi0 = &i2;
      }
      t.v0 = *pi0;
      t.v1 = *pi1;
      t.v2 = *pi2;
      if (HalfPlaneSign(points[t.v0], points[t.v1], points[t.v2]) < 0.0f)
      {
        swap(t.v1, t.v2);
      }
      triangles.push_back(t);
    }
    else
    {

    }
  }
  else
  {
    int h = (l + r) / 2;
    if (r - l == 4)
    {
      h++;
    }
    Split(l, h);
    Split(h, r);
  }
}

void MainWindow::BruteForceVoronoi()
{
  vector<Edge> halfPlanes;
  for (int i = 0; i < points.size(); i++)
  {
    halfPlanes.clear();
    for (int j = 0; j < points.size(); j++)
    {
      if (i == j)
      {
        continue;
      }
      Vector2 center = (points[i] + points[j]) * 0.5f;
      Vector2 perp = (points[i] - points[j]).GetPerpendicular();
      if (perp.Length() == 0.0f)
      {
        continue;
      }
      Vector2 n = perp.Normalized();
      halfPlanes.push_back(Edge(center - n * 50.0f, center + n * 50.0f));
    }

    vector<Vector2> isectPoints;
    for (int k = 0; k < halfPlanes.size(); k++)
    {
      for (int l = k + 1; l < halfPlanes.size(); l++)
      {
        Vector2 r(0.0f, 0.0f);
        if (IntersectLines(halfPlanes[k].p0, halfPlanes[k].p1
                         , halfPlanes[l].p0, halfPlanes[l].p1
                         , r))
        {
          bool inside = true;
          for (int j = 0; j < halfPlanes.size(); j++)
          {
            if (j == k || j == l)
            {
              continue;
            }
            if (HalfPlaneSign(halfPlanes[j].p0, halfPlanes[j].p1, points[i]) *
                HalfPlaneSign(halfPlanes[j].p0, halfPlanes[j].p1, r) <= 0.0f)
            {
              inside = false;
            }
          }
          if (inside)
          {
            isectPoints.push_back(r);
          }
        }
      }
    }

    CalcConvexHull(isectPoints);
    for (int j = 0; j < isectPoints.size(); j++)
    {
//      gIsectPoints.push_back(isectPoints[j]);
      edges.push_back(Edge(isectPoints[j], isectPoints[(j + 1) % isectPoints.size()]));
    }

  }
}
