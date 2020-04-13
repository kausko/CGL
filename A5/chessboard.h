#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ChessBoard; }
QT_END_NAMESPACE

class ChessBoard : public QMainWindow
{
    Q_OBJECT

public:
    ChessBoard(QWidget *parent = nullptr);
    void redraw();
    void dda(float,float,float,float);
    void floodFill(float, float, QRgb, QRgb);
    ~ChessBoard();

private slots:
    void on_orderSpinBox_valueChanged(int arg1);
    void on_dimSpinBox_valueChanged(int arg1);
    void on_verticalSlider_valueChanged(int value);

    void on_angleSpinBox_valueChanged(int arg1);

private:
    Ui::ChessBoard *ui;
    QRgb linecolor, fillcolor, bgcolor;
    float baseX, baseY, order, dim, angle;
};
#endif // CHESSBOARD_H
