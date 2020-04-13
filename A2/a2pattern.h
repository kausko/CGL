#ifndef A2PATTERN_H
#define A2PATTERN_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class A2Pattern; }
QT_END_NAMESPACE

class A2Pattern : public QMainWindow
{
    Q_OBJECT

public:
    A2Pattern(QWidget *parent = nullptr);
    void checkFields();
    void BresenhamCircle(int, int, int);
    void drawCircle(int, int, int, int);
    void DDACircle(float, float, float);
    void dda(double, double, double, double);
    ~A2Pattern();

private slots:
    void on_drawPushButton_pressed();
    void on_textEditX_textChanged();
    void on_textEditY_textChanged();
    void on_textEditR_textChanged();

private:
    Ui::A2Pattern *ui;
};
#endif // A2PATTERN_H
