#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    std::vector<std::vector<double> > polyTranslate(double, double);
    std::vector<std::vector<double> > polyRotate(double);
    std::vector<std::vector<double> > polyMul(std::vector<std::vector<double> >, std::vector<std::vector<double> >);
    std::pair<double,double> calcCentroid(std::vector<std::vector<double> >);
    void dda(double,double,double,double,QRgb);
    void drawPoly(std::vector<std::vector<double> >);
    void mousePressEvent(QMouseEvent *);
    ~MainWindow();

private slots:
    void on_Buttonx_0_pressed();
    void on_Buttony_0_pressed();
    void on_Buttonx_y_pressed();
    void on_rotateButton_pressed();
    void on_csabButton_pressed();

private:

    std::vector<std::vector<double> > vecs;
    double stx,sty,currx,curry;
    bool flag;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
