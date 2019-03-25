#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "config.h"
#include <mem.h>
#include <QDebug>
#include <cpu.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),splineSeries_m(new QSplineSeries()),splineSeries_s(new QSplineSeries()),splineSeries_cpu0(new QSplineSeries()),splineSeries_cpu1(new QSplineSeries()),

    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    //关联定时器溢出信号和相应的槽函数
    connect(timer,&QTimer::timeout, this, &MainWindow::getMemAvailable);
    connect(timer,&QTimer::timeout, this, &MainWindow::getRuntime);
    connect(timer,&QTimer::timeout, this, &MainWindow::showNowTime);
    connect(timer,&QTimer::timeout, this, &MainWindow::getSwapFree);
    connect(timer,&QTimer::timeout, this, &MainWindow::showMemline);
    connect(timer,&QTimer::timeout, this, &MainWindow::getCpu0);
    connect(timer,&QTimer::timeout, this, &MainWindow::getCpu1);
    connect(timer,&QTimer::timeout, this, &MainWindow::showCpuline);
    connect(timer,&QTimer::timeout, this, &MainWindow::showprocess);
    timer->start(1000);

//    QTimer *timer2 = new QTimer(this);
//    //关联定时器溢出信号和相应的槽函数
//    connect(timer2,&QTimer::timeout, this, &MainWindow::getCpu0);
//    connect(timer2,&QTimer::timeout, this, &MainWindow::getCpu1);
//    timer2->start(500);

    QStringList widget;
    widget<<"PID"<<"NAME"<<"PPID"<<"STATE"<<"MEM(KB)";
    ui->tableWidget->setHorizontalHeaderLabels(widget);

    ui->label->setText("Hostname:");
    ui->label_2->setText(Config::getHostname());
    ui->label_3->setText("Start-up Time:");
    ui->label_4->setText(Config::getStarttime());
    ui->label_5->setText("Run Time:");
    ui->label_7->setText("System version:");
    ui->label_8->setText(Config::getSysversion());
    ui->label_9->setText("Cpu information:");
    ui->label_10->setText(Config::getCpuinfo());
    ui->progressBar->setRange(0,100);

    Mem::getMemTotal();
    ui->label_12->setText(Mem::MemTotal);
    Mem::getSwapTotal();
    ui->label_22->setText(Mem::SwapTotal);
    ui->label_29->setStyleSheet("background-color:#ff0000");
    ui->label_28->setStyleSheet("background-color:#030303");

    //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(on_pushButton_clicked()));
    //connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(on_pushButton_2_clicked()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), ui->pushButton_5, SIGNAL(clicked()), Qt::UniqueConnection);

    QChart *chart = new QChart();
    chart->addSeries(splineSeries_m);
    chart->addSeries(splineSeries_s);
    chart->legend()->hide();
    chart->setTitle("Mem&Swap usage");
    chart->createDefaultAxes(); // 默认没有创建坐标轴
    chart->axisY()->setRange(0, 100);
    chart->axisX()->setRange(-120,0);

    ui->chartview->setChart(chart);
    ui->chartview->setRenderHint(QPainter::Antialiasing);

    QChart *chart_cpu = new QChart();
    chart_cpu->addSeries(splineSeries_cpu0);
    chart_cpu->addSeries(splineSeries_cpu1);
    chart_cpu->legend()->hide();
    chart_cpu->setTitle("cpu usage");
    chart_cpu->createDefaultAxes(); // 默认没有创建坐标轴
    chart_cpu->axisY()->setRange(0, 100);
    chart_cpu->axisX()->setRange(-120,0);
    ui->chartview_2->setChart(chart_cpu);
    ui->chartview_2->setRenderHint(QPainter::Antialiasing);

    ui->label_33->setStyleSheet("background-color:#ff0000");
    ui->label_34->setStyleSheet("background-color:#030303");
    ui->label_32->setStyleSheet("color:#ff0000");
    ui->label_26->setStyleSheet("color:#ff0000");

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//列均匀

    QLabel *per = new QLabel("By Darkness", this);
    ui->statusBar->addPermanentWidget(per);
    //delete per;
}

MainWindow::~MainWindow()
{
    delete splineSeries_s;
    delete splineSeries_m;
    delete ui;
}

void MainWindow::getMemAvailable()
{
    Mem::MemAvailable.clear();
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error";
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull())
    {
        if(line.contains("MemAvailable"))
        {
            for(int i = 0;i<line.length();i++)
            {
                if(line[i] >= '0' && line[i] <= '9')
                    Mem::MemAvailable.append(line[i]);
            }
        }
        line = in.readLine();
    }

    ui->label_14->setText(Mem::MemAvailable);
    ui->label_18->setText(QString::number(Mem::MemTotal.toInt() - Mem::MemAvailable.toInt()));
    int Memusage = 100 - 100 * Mem::MemAvailable.toInt()/Mem::MemTotal.toInt();
    ui->progressBar->setValue(Memusage);

    Line temp;
    temp.x = 0;
    temp.y = Memusage;
    if(Musage.size() >= 120)
        Musage.pop_front();
    Musage.push_back(temp);
}

void MainWindow::getRuntime()
{
    QFile file("/proc/uptime");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug()<<"error";
        return ;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QString str;
    for(int i = 0;i<line.length();i++)
    {
        if(line[i] != '.')
            str.append(line[i]);
        else
            break;
    }
    str = formatTime(str.toInt());
    ui->label_6->setText(str);
}

QString MainWindow::formatTime(int s)
{

   int ss = 1;
   int mi = ss * 60;
   int hh = mi * 60;
   int dd = hh * 24;

   long day = s / dd;
   long hour = (s - day * dd) / hh;
   long minute = (s - day * dd - hour * hh) / mi;
   long second = (s - day * dd - hour * hh - minute * mi) ;

   QString hou = QString::number(hour,10);
   QString min = QString::number(minute,10);
   QString sec = QString::number(second,10);
   return hou + ":" + min + ":" + sec ;
}

void MainWindow::on_pushButton_clicked()
{
    QProcess *proc = new QProcess;
    proc->start("shutdown -h now");
}

void MainWindow::on_pushButton_2_clicked()
{
    QProcess *proc = new QProcess;
    proc->start("shutdown -r now");
}

void MainWindow::showNowTime()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss");
    ui->statusBar->showMessage(current_date,1000);
}

void MainWindow::getSwapFree()
{
    Mem::SwapFree.clear();
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error";
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull())
    {
        if(line.contains("SwapFree"))
        {
            for(int i = 0;i<line.length();i++)
            {
                if(line[i] >= '0' && line[i] <= '9')
                    Mem::SwapFree.append(line[i]);
            }
        }
        line = in.readLine();
    }
    ui->label_23->setText(QString::number(Mem::SwapTotal.toInt() - Mem::SwapFree.toInt()));
    int Swapusage = 100 - 100 * Mem::SwapFree.toInt()/Mem::SwapTotal.toInt();
    ui->progressBar_2->setValue(Swapusage);

    Line temp;
    temp.x = 0;
    temp.y = Swapusage;
    if(Susage.size() >= 120)
        Susage.pop_front();
    Susage.push_back(temp);
}

void MainWindow::showMemline()
{
   // QSplineSeries *splineSeries_m = new QSplineSeries();
    splineSeries_m->clear();
    for(std::deque<Line>::iterator myItor = Musage.begin() ; myItor!=Musage.end(); myItor++)
    {
        splineSeries_m->append(myItor->x,myItor->y);
        (myItor->x)--;
    }
    splineSeries_m->setColor("#FF0000");

   // QSplineSeries *splineSeries_s = new QSplineSeries();
    splineSeries_s->setName("spline");
    splineSeries_s->clear();
    for(std::deque<Line>::iterator myItor = Susage.begin() ; myItor!=Susage.end(); myItor++)
    {
        splineSeries_s->append(myItor->x,myItor->y);
        (myItor->x)--;
    }
    splineSeries_s->setColor("#030303");

    //delete splineSeries_s;
}

void MainWindow::getCpu0()
{
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error";
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QString total;
    int total_num = 0;
    QString free;
    while (!line.isNull())
    {
        if(line.contains("cpu0"))
        {
            int blank = 0;
            for(int i = 0;i<line.length();i++)
            {
                if(line[i] >= '0' && line[i] <= '9')
                    total.append(line[i]);
                if(blank == 4 && line[i] >= '0' && line[i] <= '9')
                    free.append(line[i]);
                if(line[i] == ' ')
                {
                    blank++;
                    total_num += total.toInt();
                    total.clear();
                }
            }
        }
        line = in.readLine();
    }
    int cpu0usage = 0;
    if(Cpu::cpu0total.size() != 0)
    {
        int t = total_num - Cpu::cpu0total.toInt();
        int f = free.toInt() - Cpu::cpu0free.toInt();
        cpu0usage = (100 - 100*f/t);
        Line temp;
        temp.x = 0;
        temp.y = cpu0usage;
        if(Cpu0usage.size() >= 120)
            Cpu0usage.pop_front();
        Cpu0usage.push_back(temp);
    }
    Cpu::cpu0total = QString::number(total_num);
    Cpu::cpu0free = free;
    ui->progressBar_3->setValue(cpu0usage);
}

void MainWindow::getCpu1()
{
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error";
        return;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QString total;
    int total_num = 0;
    QString free;
    while (!line.isNull())
    {
        if(line.contains("cpu1"))
        {
            int blank = 0;
            for(int i = 0;i<line.length();i++)
            {
                if(line[i] >= '0' && line[i] <= '9')
                    total.append(line[i]);
                if(blank == 4 && line[i] >= '0' && line[i] <= '9')
                    free.append(line[i]);
                if(line[i] == ' ')
                {
                    blank++;
                    total_num += total.toInt();
                    total.clear();
                }
            }
        }
        line = in.readLine();
    }

    int cpu1usage = 0;
    if(Cpu::cpu1total.size() != 0)
    {
        int t = total_num - Cpu::cpu1total.toInt();
        int f = free.toInt() - Cpu::cpu1free.toInt();
        cpu1usage = (100 - 100*f/t);
        Line temp;
        temp.x = 0;
        temp.y = cpu1usage;
        if(Cpu1usage.size() >= 120)
            Cpu1usage.pop_front();
        Cpu1usage.push_back(temp);
    }
    Cpu::cpu1total = QString::number(total_num);
    Cpu::cpu1free = free;
    ui->progressBar_4->setValue(cpu1usage);
}

void MainWindow::showCpuline()
{
    splineSeries_cpu0->clear();
    for(std::deque<Line>::iterator myItor = Cpu0usage.begin() ; myItor!=Cpu0usage.end(); myItor++)
    {
        splineSeries_cpu0->append(myItor->x,myItor->y);
        (myItor->x)--;
    }
    splineSeries_cpu0->setColor("#FF0000");

   // QSplineSeries *splineSeries_s = new QSplineSeries();
    splineSeries_cpu1->setName("spline");
    splineSeries_cpu1->clear();
    for(std::deque<Line>::iterator myItor = Cpu1usage.begin() ; myItor!=Cpu1usage.end(); myItor++)
    {
        splineSeries_cpu1->append(myItor->x,myItor->y);
        (myItor->x)--;
    }
    splineSeries_cpu1->setColor("#030303");
}

void MainWindow::showprocess()
{
      string_list.clear();
     //判断路径是否存在
      QDir dir("/proc");
      if(!dir.exists())
          return;

      //统计cfg格式的文件个数
      int dir_count = dir.count();
      if(dir_count <= 0)
          return;

      //存储文件名称
      for(int i=0; i<dir_count; i++)
      {
          QString file_name = dir[i];  //文件名称
          if(file_name[0] >= '0' && file_name[0] <= '9')
             string_list.append(file_name);
      }
      string_list.sort();

      ui->tableWidget->setRowCount(string_list.size());

      processNum = string_list.size();
      for(int i = 0;i<string_list.size();i++)
      {
          QString name,pid,ppid,rss,state;
          QFile file("/proc/"+string_list[i]+"/status");
          if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          {
              qDebug()<<"error";
              return;
          }
          QTextStream in(&file);
          QString line = in.readLine();

          int pflag = 0;      //To avoid reading Pid more times
          while (!line.isNull())
          {
              if(line.contains("Name"))
              {
                      int nflag = 0;
                      for(int j = 0;j<line.length();j++)
                      {
                          if(nflag == 1 && line[j] != ' ')
                              name.append(line[j]);
                          if(line[j] == ':')
                              nflag = 1;
                      }
              }
              else if(line.contains("PPid"))
              {
                      for(int j = 0;j<line.length();j++)
                      {
                          if(line[j] >= '0' && line[j] <= '9')
                             ppid.append(line[j]);
                      }
              }
              else if(line.contains("Pid"))
              {
                  if(pflag == 0)
                  {
                      for(int j = 0;j<line.length();j++)
                      {
                          if(line[j] >= '0' && line[j] <= '9')
                              pid.append(line[j]);
                          //qDebug()<<pid;
                      }
                      pflag = 1;
                  }
              }
              else if(line.contains("RssAnon"))
              {
                      for(int j = 0;j<line.length();j++)
                      {
                          if(line[j] >= '0' && line[j] <= '9')
                             rss.append(line[j]);
                      }
              }
              else if(line.contains("State"))
              {
                      int sflag = 0;
                      for(int j = 0;j<line.length();j++)
                      {
                          if(sflag == 1 && line[j] != ' ')
                              state.append(line[j]);
                          if(line[j] == ':')
                              sflag = 1;
                      }
              }
              line = in.readLine();
          }
           ui->tableWidget->setItem(i,0,new QTableWidgetItem(pid.trimmed()));
           ui->tableWidget->setItem(i,1,new QTableWidgetItem(name.trimmed()));
           ui->tableWidget->setItem(i,2,new QTableWidgetItem(ppid.trimmed()));
           ui->tableWidget->setItem(i,3,new QTableWidgetItem(state.trimmed()));
           ui->tableWidget->setItem(i,4,new QTableWidgetItem(rss.trimmed()));
      }
}


void MainWindow::on_pushButton_4_clicked()
{
       if(ui->tableWidget->selectedItems().isEmpty()) return;
       QMessageBox::StandardButton rb = QMessageBox::question(NULL, "Warning", "This maybe  will broke your system,are you sure to continue?", QMessageBox::Yes | QMessageBox::No);
       if(rb == QMessageBox::Yes)
       {
           //QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
           int rownum = ui->tableWidget->currentItem()->row();
           //QTableWidgetItem *item = items.at(0);
           QString pid=ui->tableWidget->item(rownum,0)->text();
           QProcess::execute("kill "+pid);
           return ;
       }
       return ;
}

void MainWindow::on_pushButton_5_clicked()
{
      QString text=ui->lineEdit->text();
      if(text.isEmpty())
          return;
      int find = 0;
      for(int i = 0;i<processNum;i++)
      {
          if(ui->tableWidget->item(i,0)->text()==text || ui->tableWidget->item(i,1)->text()==text)
          {
              ui->tableWidget->selectRow(i);
              find = 1;
              break;
          }
      }
      if(find == 0)
          QMessageBox::about(NULL, "Error", "No matched process!");
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->tableWidget->selectedItems().isEmpty()) return;
    int rownum = ui->tableWidget->currentItem()->row();
    QString pid=ui->tableWidget->item(rownum,0)->text();

    QFile file("/proc/"+pid+"/status");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error";
        return;
    }
    QTextStream in(&file);
    QString infoStr = in.readAll();
    QMessageBox::about(NULL,"Detail",infoStr.trimmed());
}
