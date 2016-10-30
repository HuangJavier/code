#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "greedy.h"
#include "backfire.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    started = false;

    //在这里加入算法
    tsps.push_back(new Greedy);

    tsps.push_back(new BackFire);

    ui->tableWidget->setRowCount(tsps.size());
    for (size_t i = 0;i < tsps.size();++i){
        TSP *tsp = tsps[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(tsp->name)));
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    for (TSP *tsp : tsps){
        delete tsp;
    }
    delete ui;
}

void MainWindow::OnTimer(){
    for (size_t i = 0;i < tsps.size();++i){
        TSP *tsp = tsps[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(tsp->name)));
        vector<int> path;
        double bestDis = tsp->GetBestPath(path);

        if (bestDis < DBL_MAX)
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(bestDis)));
        else
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem("INF"));

        if (path.empty()){
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem("None"));
        }else{
            string pathStr;
            bool first = true;
            char buf[16];
            for (int p : path){
                if (!first)pathStr += ", ";
                first = false;
                sprintf(buf, "%d", p);
                pathStr += buf;
            }
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(pathStr)));
        }
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString("%1").arg(tsp->iter_time)));
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter;
    painter.begin(this);
    double minX = INT_MAX;
    double minY = INT_MAX;
    double maxX = INT_MIN;
    double maxY = INT_MIN;
    for (Point &p : TSP::points){
        minX = min(minX, p.x);
        minY = min(minY, p.y);
        maxX = max(maxX, p.x);
        maxY = max(maxY, p.y);
    }
    double dx = maxX - minX;
    double dy = maxY - minY;
    double dz = max(dx, dy);
    double ratio = 600.0 / dz;
    QPen qpen(Qt::green, 5, Qt::SolidLine);
    QPen tpen(Qt::black, 1, Qt::SolidLine);
    const int offsetX = 50;
    const int offsetY = 50;
    for (int i = 0;i < TSP::points.size();++i){
        Point &p = TSP::points[i];
        int x = (p.x - minX) * ratio + offsetX;
        int y = (p.y - minY) * ratio + offsetY;
        painter.setPen(qpen);
        painter.drawPoint(x,y);
        painter.setPen(tpen);
        painter.drawText(x + 5,y,QString("%1").arg(i));
    }

    int idx = ui->tableWidget->currentIndex().row();
    if (idx >= 0){
        QPen qpen(Qt::black, 1, Qt::SolidLine);
        painter.setPen(qpen);
        TSP *tsp = tsps[idx];
        vector<int> bestPath;
        double BestDis = tsp->GetBestPath(bestPath);
        if (!bestPath.empty()){
            for (int i = 1;i < bestPath.size();++i){
                int id1 = bestPath[i-1];
                int id2 = bestPath[i];
                Point st = TSP::points[id1];
                Point en = TSP::points[id2];
                QPoint q1((st.x - minX) * ratio + offsetX, (st.y - minY) * ratio + offsetY);
                QPoint q2((en.x - minX) * ratio + offsetX, (en.y - minY) * ratio + offsetY);
                painter.drawLine(q1, q2);
            }
            Point st = TSP::points[bestPath[TSP::points.size() - 1]];
            Point en = TSP::points[bestPath[0]];
            QPoint q1((st.x - minX) * ratio + offsetX, (st.y - minY) * ratio + offsetY);
            QPoint q2((en.x - minX) * ratio + offsetX, (en.y - minY) * ratio + offsetY);
            painter.drawLine(q1, q2);
        }
    }

    painter.end();
}

void MainWindow::on_pushButton_clicked()
{
    if (!started){
        QString filename = QFileDialog::getOpenFileName(this, "Select a *.ftsp file", ".", "*.ftsp");
        if (filename.size()){
            //读取TSP DATA
            TSP::read(filename.toStdString());
            for (TSP *tsp : tsps){
                tsp->start();
            }
            ui->pushButton->setText("Finish");
            ui->pushButton->setEnabled(false);
            started = true;
        }else{
            QMessageBox::information(NULL, tr("No File"),tr("No Fixed TSP File :-("));
        }
    }else{
        for (TSP *tsp : tsps){
            tsp->quit();
        }
    }
}
