#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    double stx,sty,currx,curry;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void mousePressEvent(QMouseEvent *);
    void dda(float,float,float,float);
    void floodFill(float, float, QRgb, QRgb);

private slots:
    void on_pushButtonCl_pressed();

private:
    Ui::MainWindow *ui;
    int flag;
    QColor fillcolor;
    QColor linecolor;
};

#endif // MAINWINDOW_H
