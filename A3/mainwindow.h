#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bits/stdc++.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<std::vector<double> > polyTranslate(double, double);
    std::vector<std::vector<double> > polyScale(double);
    std::vector<std::vector<double> > polyRotate(double);
    std::vector<std::vector<double> > polyMul(std::vector<std::vector<double> >, std::vector<std::vector<double> >);
    std::pair<double,double> calcCentroid(std::vector<std::vector<double> >);
    void dda(double,double,double,double,QRgb);
    void drawPoly(std::vector<std::vector<double> >);
    void mousePressEvent(QMouseEvent *);
    ~MainWindow();

private slots:
    void on_pushButton_pressed();
    void on_comboBox_currentIndexChanged(int index);
    void on_csabButton_pressed();

private:
    std::vector<std::vector<double> > vecs;
    double stx,sty,currx,curry;
    bool flag;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
