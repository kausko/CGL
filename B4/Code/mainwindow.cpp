#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QColorDialog>
#include <QPalette>
#include <math.h>
#define xoff 680
#define yoff 330

QImage image(1360,660, QImage::Format_RGB888);
QColor oldColor = Qt::white;
QColor newColor = Qt::yellow;
QColor graphColor = Qt::green;

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow){
    flag = false;
    ui->setupUi(this);
    dda(xoff/2,0,3*xoff/2,660,graphColor.rgb());
    dda(0,yoff,1360,yoff,graphColor.rgb());
    dda(xoff,0,xoff,660,graphColor.rgb());
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->csabButton->setEnabled(false);
}

MainWindow::~MainWindow(){
    delete ui;
}


void MainWindow::on_Buttonx_0_pressed(){
    drawPoly(polyMul(vecs,{{1,0,0},{0,-1,0},{0,0,1}}));
}

void MainWindow::on_Buttony_0_pressed(){
    drawPoly(polyMul(vecs,{{-1,0,0},{0,1,0},{0,0,1}}));
}

void MainWindow::on_Buttonx_y_pressed(){
    drawPoly(polyMul(vecs,{{0,1,0},{1,0,0},{0,0,1}}));
}

void MainWindow::on_rotateButton_pressed(){
    QString xstr = ui->textEditX->toPlainText();
    QString ystr = ui->textEditY->toPlainText();
    QString thetastr = ui->textEditTheta->toPlainText();
    if(xstr=="" || ystr=="" || thetastr == "")
        return;
    else{
        double x = xstr.toDouble();
        double y = ystr.toDouble();
        double theta = thetastr.toDouble();
        drawPoly(polyMul(polyMul(polyMul(vecs,polyTranslate(-x,-y)),polyRotate(theta)),polyTranslate(x,y)));
    }
}

void MainWindow::on_csabButton_pressed(){
    vecs.clear();
    flag = false;
    ui->csabButton->setEnabled(false);
    image.fill(Qt::black);
    dda(xoff/2,0,3*xoff/2,660,graphColor.rgb());
    dda(0,yoff,1360,yoff,graphColor.rgb());
    dda(xoff,0,xoff,660,graphColor.rgb());
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::mousePressEvent(QMouseEvent * ev){
    if(ev->buttons() == Qt::LeftButton){
        if(!flag){
            stx = currx = ev->x();
            sty = curry = ev->y();
            flag = true;
        }
        else{
            dda(currx,curry,ev->x(),ev->y(),oldColor.rgb());
            currx = ev->x();
            curry = ev->y();
        }
        std::vector<double> vec = {currx-xoff,curry-yoff,1};
        vecs.push_back(vec);
        ui->csabButton->setEnabled(true);
    }
    else if(ev->buttons() == Qt::RightButton){
        dda(currx, curry, stx, sty, oldColor.rgb());
        flag = false;
    }
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::dda(double x1, double y1, double x2, double y2, QRgb value){
    double dx = x2-x1;
    double dy = y2-y1;
    double length = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    double delx = dx/length;
    double dely = dy/length;

    double x = x1 + 0.5*sign(delx);
    double y = y1 + 0.5*sign(dely);

    for(int i=0; i<= length; i++){
        image.setPixel(x,y,value);
        x += delx;
        y += dely;
    }

}

std::vector<std::vector<double> > MainWindow::polyTranslate(double ipx, double ipy){

    //std::pair<double,double> centroid = calcCentroid(vecs);

    //double transfx = ipx - centroid.first;
    //double transfy = ipy - centroid.second;

    return std::vector<std::vector<double> > {
        {1,0,0},
        {0,1,0},
        {ipx,ipy,1}
    };
}

std::vector<std::vector<double> > MainWindow::polyRotate(double angle){
    double rad = angle * M_PI/180;
    double cosa = std::cos(rad);
    double sina = std::sin(rad);
    return std::vector<std::vector<double> > {
        {cosa, sina, 0},
        {-sina, cosa, 0},
        {0, 0, 1}
    };
}

std::pair<double,double> MainWindow::calcCentroid(std::vector<std::vector<double> > vec){

    std::pair<double, double> centroid = { 0, 0 };  //  Cx, Cy

    double signedArea = 0;
    unsigned long rowcnt = vec.size();
    for(unsigned long i=0; i < rowcnt; i++){
        double x0 = vec[i][0];
        double y0 = vec[i][1];
        double x1 = vec[(i + 1) % rowcnt][0];
        double y1 = vec[(i + 1) % rowcnt][1];
        double A = (x0 * y1) - (x1 * y0);
        signedArea += A;
        centroid.first += (x0 + x1) * A;
        centroid.second += (y0 + y1) * A;
    }

    signedArea *= 0.5;
    centroid.first = (centroid.first) / (6 * signedArea);
    centroid.second = (centroid.second) / (6 * signedArea);

    return centroid;
}

std::vector<std::vector<double> > MainWindow::polyMul(std::vector<std::vector<double> > m1, std::vector<std::vector<double> > m2){
    int rowcnt1 = m1.size();
    int rowcnt2 = m2.size();
    std::vector<std::vector<double> > res( rowcnt1 , std::vector<double> (3, 0));
    for(int i=0; i<rowcnt1; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<rowcnt2; k++){
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return res;
}

void MainWindow::drawPoly(std::vector<std::vector<double> > final){
    int i=0, rowcnt = final.size();
    do{
        dda(final[i][0]+xoff, final[i][1]+yoff, final[(i+1)%rowcnt][0]+xoff, final[(i+1)%rowcnt][1]+yoff, newColor.rgb());
        i = (i+1)%rowcnt;
    }
    while(i!=0);
    ui->label->setPixmap(QPixmap::fromImage(image));
}
