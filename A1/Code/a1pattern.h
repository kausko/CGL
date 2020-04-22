#ifndef A1PATTERN_H
#define A1PATTERN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class A1Pattern; }
QT_END_NAMESPACE

class A1Pattern : public QMainWindow
{
    Q_OBJECT

public:
    A1Pattern(QWidget *parent = nullptr);
    void initQDraw();
    void initPDraw();
    void enablePBs();// checks text fields to enable pushButtons
    void drawPattern(int, int, int, int);
    void dda(double, double, double, double);
    void bresenham(int, int, int, int);
    ~A1Pattern();

private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_QDrawBresPB_pressed();
    void on_ddaPB_pressed();
    void on_textEditX1_textChanged();
    void on_textEditY1_textChanged();
    void on_textEditX2_textChanged();
    void on_textEditY2_textChanged();

private:
    Ui::A1Pattern *ui;
    //QImage * image;
    QColor value = Qt::white;
};
#endif // A1PATTERN_H
