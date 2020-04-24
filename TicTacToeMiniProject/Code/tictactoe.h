#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QMainWindow>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class TicTacToe; }
QT_END_NAMESPACE

class TicTacToe : public QMainWindow
{
    Q_OBJECT

public:
    TicTacToe(QWidget *parent = nullptr);
    void renewGame();
    void checkwin();
    void updateScore();
    void disableAll(bool);
    void setWin(QPushButton *,QPushButton *,QPushButton *);
    void setupNewGame(QMenu *);
    void setupUndoButton(QMenu *);
    void buttonPressed(QPushButton *);
    void turnIndicator();
    QString getChar();
    void dda(float,float,float,float);
    void floodFill(float, float, QRgb, QRgb);
    ~TicTacToe();

private slots:
    void on_pushButton00_pressed();
    void on_pushButton01_pressed();
    void on_pushButton02_pressed();
    void on_pushButton10_pressed();
    void on_pushButton11_pressed();
    void on_pushButton12_pressed();
    void on_pushButton20_pressed();
    void on_pushButton21_pressed();
    void on_pushButton22_pressed();
    void on_textEditP1_textChanged();
    void on_textEditP2_textChanged();

    void on_pushButtonNG_pressed();

private:
    Ui::TicTacToe *ui;
    int moveCntr, liveCntr;
    std::vector<QPushButton *> moves;
    QLine * line;
    QAction * newGame, * undoGame;
};
#endif // TICTACTOE_H
