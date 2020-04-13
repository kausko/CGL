#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QColorDialog>
#include <QPalette>
#include <math.h>

QImage image(1291,661, QImage::Format_RGB888);
QColor oldColor = Qt::white;
QColor newColor = Qt::yellow;

template<class T>
T sign(T x){
    if(!x)
        return 0;
    if(x>0)
        return 1;
    return -1;
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{
    flag = false;
    ui->setupUi(this);
    ui->label->setPixmap(QPixmap::fromImage(image));
    ui->csabButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_pressed()
{
    if(ui->textEditX->toPlainText()!="" && ui->textEditY->toPlainText()!="" && !vecs.empty()){
        int index = ui->comboBox->currentIndex();
        double ip1 = ui->textEditX->toPlainText().toDouble();
        double ip2 = ui->textEditY->toPlainText().toDouble();
        switch (index) {
            case 0:
                //polyTranslate(ip1, ip2);
                drawPoly(polyMul(vecs,polyTranslate(ip1,ip2)));
                break;
            case 1: {
                //polyScale(ip1);
                double cx = calcCentroid(vecs).first;
                double cy = calcCentroid(vecs).second;
                drawPoly(polyMul(polyMul(polyMul(vecs,polyTranslate(0, 0)),polyScale(ip1)),polyTranslate(2*cx,2*cy)));
                //                      Original vect * 0,0 translation * Scaling operation * counter translation
                break;
            }
            case 2: {
                //polyRotate(ip1);
                double cx = calcCentroid(vecs).first;
                double cy = calcCentroid(vecs).second;
                drawPoly(polyMul(polyMul(polyMul(vecs,polyTranslate(0,0)),polyRotate(ip1)),polyTranslate(2*cx,2*cy)));
                //                      Original vect * 0,0 translation * Rotation operation * counter translation
                break;
            }
        }
    }
}

void MainWindow::on_comboBox_currentIndexChanged(int index){
    switch (index) {
    case 0:
        ui->labelX->setText("Translate to        x:");
        ui->labelY->setVisible(true);
        ui->textEditY->setVisible(true);
        break;
    case 1:
        ui->labelX->setText("Scaling factor:");
        ui->labelY->setVisible(false);
        ui->textEditY->setVisible(false);
        break;
    case 2:
        ui->labelX->setText("Rotation angle:");
        ui->labelY->setVisible(false);
        ui->textEditY->setVisible(false);
        break;
    }

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
        std::vector<double> vec = {currx,curry,1};
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
        image.setPixel(x-10,y-10,value);
        x += delx;
        y += dely;
    }

}

std::vector<std::vector<double> > MainWindow::polyTranslate(double ipx, double ipy){

    std::pair<double,double> centroid = calcCentroid(vecs);

    double transfx = ipx - centroid.first;
    double transfy = ipy - centroid.second;

    return std::vector<std::vector<double> > {
        {1,0,0},
        {0,1,0},
        {transfx,transfy,1}
    };
}

std::vector<std::vector<double> > MainWindow::polyScale(double scaleFactor){
    return std::vector<std::vector<double> > {
        {scaleFactor, 0, 0},
        {0, scaleFactor, 0},
        {0,0,1}
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
        dda(final[i][0], final[i][1], final[(i+1)%rowcnt][0], final[(i+1)%rowcnt][1], newColor.rgb());
        i = (i+1)%rowcnt;
    }
    while(i!=0);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::on_csabButton_pressed(){
    vecs.clear();
    flag = false;
    ui->csabButton->setEnabled(false);
    image.fill(Qt::black);
    ui->label->setPixmap(QPixmap::fromImage(image));
}

