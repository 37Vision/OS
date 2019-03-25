#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QProcess>
#include <QLabel>
#include <deque>
#include <QSplineSeries>

class Line
{
public:
    int x;
    int y;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString formatTime(int s);
    std::deque<Line> Musage;
    std::deque<Line> Susage;
    std::deque<Line> Cpu0usage;
    std::deque<Line> Cpu1usage;
    QStringList string_list;
    QtCharts::QSplineSeries *splineSeries_m,*splineSeries_s,*splineSeries_cpu0,*splineSeries_cpu1;
    int processNum;
private:
    Ui::MainWindow *ui;
private slots:
    void getMemAvailable();
    void getRuntime();
    void showNowTime();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void getSwapFree();
    void showMemline();
    void showCpuline();
    void getCpu0();
    void getCpu1();
    void showprocess();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
};

#endif // MAINWINDOW_H
