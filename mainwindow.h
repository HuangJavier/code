#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
#include <string>
using namespace std;

#include <QMainWindow>
#include <QPen>
#include <QPainter>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "tsp.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_pushButton_clicked();
    void OnTimer();

private:
    void paintEvent(QPaintEvent *);
private:
    Ui::MainWindow *ui;
    vector<TSP*> tsps;
    bool started;
};

#endif // MAINWINDOW_H
