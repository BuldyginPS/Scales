#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>
#include <iostream>
#include <cstring>
#include <QVector>
#include <QTime>
#include <QTimer>
#include <cmath>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *tmr;
private slots:
    void updateTime();

    void on_weghtNowBtn_clicked();
    void on_startButton_clicked();
};
#endif // MAINWINDOW_H
