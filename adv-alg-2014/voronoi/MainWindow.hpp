#pragma once

#include <QMainWindow>

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  virtual void paintEvent(QPaintEvent *);
  virtual void mousePressEvent(QMouseEvent* e);

private:
  Ui::MainWindow *ui;

  void RecalculateStuff();
  void Split(int l, int r);
  void BruteForceVoronoi();
};
