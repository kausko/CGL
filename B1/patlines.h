#ifndef PATLINES_H
#define PATLINES_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PatLines; }
QT_END_NAMESPACE

class PatLines : public QMainWindow
{
    Q_OBJECT

public:
    PatLines(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *);
    void dda(float,float,float,float);
    ~PatLines();

private slots:
    void on_drawPB_pressed();

    void on_clearPB_pressed();

private:
    Ui::PatLines *ui;
    float stx,sty,currx,curry;
    int flag;
    QRgb lineColor;
};
#endif // PATLINES_H
