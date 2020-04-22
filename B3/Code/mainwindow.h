#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class Point;
class Polygon;
class MainWindow;
}

class Point
{
    int px, py;
public:
    Point(int h=0,int k=0)
    {
        px=h;
        py=k;
    }
    int x(){return px;}
    int y(){return py;}
    bool range(int x, int y, int offset_x, int offset_y);
    void addOffset(int, int);
    void change(int, int);
    friend class Polygon;
    //friend class Clipping;
};

struct Bools
{
  bool l, r, u, b;
  Bools()
  {
      l=r=b=u=false;
  }
  bool isAllFalse()
  {
      bool ret = false;
      ret = l || r || u || b;
      return !ret;
  }
  bool anding(Bools b2)
  {
      bool ret = false;
      ret = ret || (l && b2.l);
      ret = ret || (r && b2.r);
      ret = ret || (u && b2.u);
      ret = ret || (b && b2.b);

      return ret;
  }
};

class Polygon
{
    QVector<Point> allPoints;
    QRgb lineColour;
    int vertices;
public:
    Polygon()
    {
        vertices = 0;
        lineColour = qRgb(255,255,255);
    }
    void addVertex(Point p);
    Point getFront();
    Point getRear();
    bool isEmpty();
    void clearPoly();
    friend class MainWindow;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
    Polygon poly;
    Point A, B, C, D;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void mousePressEvent(QMouseEvent *m);
    void mouseReleaseEvent(QMouseEvent *m);

private:
    Ui::MainWindow *ui;
    void clipPolygon();
    Bools getBools(Point);
    Point findPrime(float m, Point p, Bools b);
};

#endif // MAINWINDOW_H
