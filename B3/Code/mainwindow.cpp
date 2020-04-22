#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QMouseEvent>

const int win_x = 500;
const int win_y = 500;

QImage image(win_x, win_y, QImage::Format_RGB888);

void Point::addOffset(int x, int y)
{
    px -= x;
    py -= y;
}

void Polygon::addVertex(Point p)
{
    allPoints.push_back(p);
    vertices++;
}

void Polygon::clearPoly()
{
    vertices = 0;
    allPoints.clear();
}

bool Point::range(int x, int y, int offset_x, int offset_y)
{
    if(px>=x && px<(x + offset_x))
        if(py>=y && py<(y + offset_y))
            return true;
    return false;
}

bool Polygon::isEmpty()
{
    return vertices==0;
}

Point Polygon::getFront()
{
    if(!isEmpty())
        return allPoints[0];
    else
    {
        Point p;
        return p;
    }
}

Point Polygon::getRear()
{
    if(!isEmpty())
        return allPoints[vertices-1];
    else
    {
        Point p;
        return p;
    }
}

void Point::change(int xn, int yn)
{
    px = xn;
    py = yn;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->window->setPixmap(QPixmap::fromImage(image));
    ui->window->show();
}

int sign(float val)
{
    if(val>0)
        return 1;
    else if(val<0)
        return -1;
    else
        return 0;
}

void drawLineDDA(Point p1, Point p2, QRgb value = qRgb(0,255,255))
{
    int x1 = p1.x(), x2 = p2.x(), y1 = p1.y(), y2 = p2.y();
    int length;
    if(abs(x2-x1) > abs(y2-y1))
        length = abs(x2-x1);
    else
        length = abs(y2-y1);
    float delta_x = (x2-x1)/float(length);
    float delta_y = (y2-y1)/float(length);

    float x = x1 + 0.5*sign(delta_x);
    float y = y1 + 0.5*sign(delta_y);

    for(int i = 0; i< length; i++)
    {
        image.setPixel(int(floor(x)),int(floor(y)), value);
        x = x + delta_x;
        y = y + delta_y;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *m)
{
    int x = m->x();
    int y = m->y();
    Point p(x, y);
    if(p.range(20, 20, win_x,win_y))
    {
        p.addOffset(20, 20);
        if(m->button() == Qt::LeftButton)
        {
            if(poly.isEmpty())
                poly.addVertex(p);
        }
        else if(m->button() == Qt::RightButton)
        {
            if(poly.isEmpty())
                poly.addVertex(p);
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *m)
{
    Point p(m->x(), m->y());
    if(p.range(20, 20, win_x,win_y))
    {
        p.addOffset(20, 20);
        if(m->button() == Qt::LeftButton)
        {
            drawLineDDA(p, poly.getRear(), poly.lineColour);
            poly.addVertex(p);
        }
        else if(m->button() == Qt::RightButton)
        {
            drawLineDDA(poly.getFront(), poly.getRear(), poly.lineColour);
            poly.addVertex(poly.getFront());
        }
    }
    ui->window->setPixmap(QPixmap::fromImage(image));
    ui->window->show();
}
void clearScreen()
{
    for(int i = 0; i<win_x; i++)
        for(int j = 0; j<win_y; j++)
            image.setPixel(i, j, qRgb(0,0,0));
}
void MainWindow::on_pushButton_clicked()
{
    int x1 = ui->x1->toPlainText().toInt();
    int y1 = ui->y1->toPlainText().toInt();
    int x2 = ui->x2->toPlainText().toInt();
    int y2 = ui->y2->toPlainText().toInt();
    clearScreen();
    A.change(x1,y1), B.change(x1, y2), C.change(x2,y2), D.change(x2, y1);
    drawLineDDA(A,B);
    drawLineDDA(B,C);
    drawLineDDA(C,D);
    drawLineDDA(D,A);
    clipPolygon();
    ui->window->setPixmap(QPixmap::fromImage(image));
    ui->window->show();
}

Bools MainWindow::getBools(Point p)
{
    Bools ret;
    if(p.x() < A.x()) ret.l = true;
    if(p.y() > B.y()) ret.b = true;
    if(p.x() > C.x()) ret.r = true;
    if(p.y() < D.y()) ret.u = true;

    return ret;
}

Point MainWindow::findPrime(float m, Point p, Bools b)
{
    Point ret(-1,-1);
    if(b.l)
    {
        int int_x = A.x();
        int int_y;
        float y = m*(int_x-p.x()) + p.y();
        int_y = floor(y);
        if(int_y > B.y() || int_y<D.y())
            return ret;
        ret.change(int_x, int_y);
        return ret;
    }
    if(b.r)
    {
        int int_x = C.x();
        int int_y;
        float y = m*(int_x-p.x()) + p.y();
        int_y = floor(y);
        if(int_y > B.y() || int_y<D.y())
            return ret;
        ret.change(int_x, int_y);
        return ret;
    }
    if(b.u)
    {
        int int_y = D.y();
        int int_x;
        float x = (int_y - p.y() + m*p.x())/m;
        int_x = floor(x);
        if(int_x>C.x() || int_x<A.x())
            return ret;
        ret.change(int_x, int_y);
        return ret;
    }
    if(b.b)
    {
        int int_y = B.y();
        int int_x;
        float x = (int_y - p.y() + m*p.x())/m;
        int_x = floor(x);
        if(int_x>C.x() || int_x<A.x())
            return ret;
        ret.change(int_x, int_y);
        return ret;
    }
}

void MainWindow::clipPolygon()
{
    if(poly.vertices)
    {
        Point p1 = poly.getFront(), p2;
        Point p1_prime, p2_prime;
        Bools b1 = getBools(p1);
        Bools b2;
        for(int v = 1; v<poly.vertices; v++)
        {
            p2 = poly.allPoints[v];
            b2 = getBools(p2);
            if(!b1.anding(b2))
            {
                float m = (p1.y() - p2.y())/float(p1.x()-p2.x());

                if(b1.isAllFalse())
                {
                    p1_prime = p1;
                }
                else
                {
                    p1_prime = findPrime(m, p1, b1);
                }

                if(b2.isAllFalse())
                {
                    p2_prime = p2;
                }
                else
                {
                    p2_prime = findPrime(m, p2, b2);
                }
                //drawLineDDA(p2, p1, qRgb(0,0,0));
                if(p1_prime.x() >= 0 && p2_prime.x() >= 0)
                    drawLineDDA(p2_prime, p1_prime, poly.lineColour);
            }
           // else
               // drawLineDDA(p2, p1, qRgb(0,0,0));
            p1 = p2;
            b1 = b2;
        }
    }
}
























MainWindow::~MainWindow()
{
    delete ui;
}

