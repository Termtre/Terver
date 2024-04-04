#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Фомичев Дмитрий 3821Б1ПМмм1");

    QPixmap pix(":/img/img/1.png");
    int w = 800;//ui->label->width();
    int h = 200;//ui->label->height() + 25;
    ui->label->setPixmap(pix.scaled(w, h));

    QPixmap pix2(":/img/img/2.png");
    int w2 = ui->label->width();
    int h2 = ui->label->height();
    ui->img2->setPixmap(pix2.scaled(w, h));

    QPixmap pix3(":/img/img/3.png");
    int w3 = ui->label->width();
    int h3 = ui->label->height();
    ui->img3->setPixmap(pix3.scaled(w3, h3));

    //ui->tabWidget->setTabText(0, "Задание 1");
    //ui->tabWidget->setTabText(1, "Задание 2");
    //ui->tabWidget->setTabText(2, "Задание 3");

    ui->tabWidget->setCurrentIndex(0);

    chart = new QChart();
    chart->legend()->setVisible(false);
    chart->setTitle("Плотность распределения c.в. η");

    axisX = new QValueAxis();
    axisX->setLabelFormat("%f");
    axisX->setTickCount(5);
    axisX->setTitleText("y");
    chart->addAxis(axisX, Qt::AlignBottom);

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(5);
    axisY->setTitleText("fη(y)");
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->raspr->setRenderHint(QPainter::Antialiasing);

    chart2 = new QChart();
    chart2->legend()->setVisible(false);
    chart2->setTitle("Функция распределения c.в. η");

    axisX2 = new QValueAxis();
    axisX2->setLabelFormat("%f");
    axisX2->setTickCount(5);
    axisX2->setTitleText("y");
    chart2->addAxis(axisX2, Qt::AlignBottom);

    axisY2 = new QValueAxis();
    axisY2->setLabelFormat("%f");
    axisY2->setTickCount(5);
    axisY2->setTitleText("Fη(y)");
    chart2->addAxis(axisY2, Qt::AlignLeft);

    chart2->layout()->setContentsMargins(0, 0, 0, 0);
    chart2->setBackgroundRoundness(0);

    ui->funct->setRenderHint(QPainter::Antialiasing);
    ui->tableWidget->verticalHeader()->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_CreateGraphics_clicked()
{
    if (!changedC)
    {
        if (rasprV || funcV)
        {
            chart->removeAllSeries();
            chart2->removeAllSeries();
            rasprV = nullptr;
            funcV = nullptr;
        }

        rasprV = new QLineSeries();
        funcV = new QLineSeries();

        *rasprV << QPointF(-2., 0.);
        *rasprV << QPointF(c, 0.);
        *rasprV << QPointF(0., 1.);

        *funcV << QPointF(-2., 0.);
        *funcV << QPointF(c, 0.);

        for (double y = c; y <= 0. ; y += 0.01)
        {
            *funcV << QPointF(y, -y * y / 2. / c + y - c / 2.);
        }

        if (c != -2.)
        {
            for (int i = 0; i < 100; i++)
            {
                double y = static_cast<double>(i) / 50.;
                *rasprV << QPointF(y, exp(-lambda * y));
            }

            for (double y = 0; y <= 2.; y += 0.01)
            {
                *funcV << QPointF(y, -exp(-lambda * y) / lambda + 1. / lambda - c / 2.);
            }
        }

        chart->addSeries(rasprV);
        rasprV->attachAxis(axisX);
        rasprV->attachAxis(axisY);

        axisX->setRange(-2., 2.);
        axisY->setRange(0., 1.);

        ui->raspr->setChart(chart);

        chart2->addSeries(funcV);
        funcV->attachAxis(axisX2);
        funcV->attachAxis(axisY2);

        axisX2->setRange(-2., 2.);
        axisY2->setRange(0., 1.);
        changedC = false;
    }

    ui->funct->setChart(chart2);

    ui->tableWidget->setRowCount(N);

    QStringList nameTable;

    for (int i = 0 ; i <= N; i++)
        nameTable << QString().number(i);

    ui->tableWidget_2->setRowCount(1);
    ui->tableWidget_2->verticalHeader()->setVisible(false);
    //ui->tableWidget_2->setVerticalHeaderLabels(nameTable);
    task1();
    task2();
}


void MainWindow::on_setC_editingFinished()
{
    bool ok;
    ui->setC->text().toDouble(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->CreateGraphics->setEnabled(false);

        return;
    }

    if (ui->setC->text().toDouble() > 0. || ui->setC->text().toDouble() < -2.)
    {
        QMessageBox msgBox;
        msgBox.setText("Параметр C не может быть больше нуля или меньше -2");
        msgBox.exec();
        ui->CreateGraphics->setEnabled(false);
        return;
    }

    ui->CreateGraphics->setEnabled(true);
    c = ui->setC->text().toDouble();
    if (c != -2.) lambda = 2. / (2. + c);
    changedC = true;
}


void MainWindow::on_lineEdit_editingFinished()
{
    bool ok;
    ui->lineEdit->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->CreateGraphics->setEnabled(false);

        return;
    }

    if (ui->lineEdit->text().toInt() < 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Количество не может быть отрицательным числом");
        msgBox.exec();
        ui->CreateGraphics->setEnabled(false);
        return;
    }

    ui->CreateGraphics->setEnabled(true);
    N = ui->lineEdit->text().toInt();
}

void MainWindow::task1()
{
    table.clear();
    for (int i = 0; i < N; i++)
    {
        double x = funcRandom(static_cast<double>(rand()) / RAND_MAX);
        table.insert({{x, plotRaspr(x)}});
    }

    int i = 0;

    for (auto it = table.begin(); it != table.end(); ++it)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(std::get<0>(*it))));
        ui->tableWidget->setItem(i++, 1, new QTableWidgetItem(QString::number(std::get<1>(*it))));
    }
}

void MainWindow::task2()
{
    double totalX = 0.;
    double totalS = 0.;
    double totalR = 0.;
    double totalMe = 0.;
    double totalM = c * c / 12. + c + 1.;
    double middleX = 0.;
    double totalD = 0.25 * pow((2. + c), 3) - pow(c, 3) / 12. - totalMe * totalMe;
    int index;

    if (table.size() % 2)
    {
        index = (table.size() - 1) / 2;
        //totalMe = std::get<0>(table[index]);
    }
    else
    {
        index = table.size()/ 2;
        //totalMe = (std::get<0>(table[index]) + std::get<0>(table[index + 1])) / 2;
    }

    int i = 0;

    for (auto it = table.begin(); it != table.end(); ++it)
    {
        if (i == index && table.size() % 2)
        {
            totalMe = std::get<0>(*it);
        }

        if (i == (index - 1) && table.size() % 2 == 0)
        {
            totalMe += std::get<0>(*it);
        }

        if (i == index && table.size() % 2 == 0)
        {
            totalMe += std::get<0>(*it);
            totalMe /= 2.;
        }

        middleX += std::get<0>(*it);
        i++;
    }


    middleX /= static_cast<double>(table.size());

    for (auto it = table.begin(); it != table.end(); ++it)
        totalS += pow((std::get<0>(*it) - middleX), 2.);

    totalS /= static_cast<double>(table.size());

    totalR = std::get<0>(*table.begin()) - std::get<0>(*(--table.end()));

    ui->tableWidget_2->setItem(0, 0, new QTableWidgetItem(QString::number(totalM)));
    ui->tableWidget_2->setItem(0, 1, new QTableWidgetItem(QString::number(middleX)));
    ui->tableWidget_2->setItem(0, 2, new QTableWidgetItem(QString::number(abs(totalM - middleX))));
    ui->tableWidget_2->setItem(0, 3, new QTableWidgetItem(QString::number(totalD)));
    ui->tableWidget_2->setItem(0, 4, new QTableWidgetItem(QString::number(totalS)));
    ui->tableWidget_2->setItem(0, 5, new QTableWidgetItem(QString::number(abs(totalD - totalS))));
    ui->tableWidget_2->setItem(0, 6, new QTableWidgetItem(QString::number(totalMe)));
    ui->tableWidget_2->setItem(0, 7, new QTableWidgetItem(QString::number(totalR)));
}

double MainWindow::plotRaspr(double y)
{   
    if (y <= c)
    {
        return 0.;
    }
    else if (c < y && y <= 0.)
    {
        return -y / c + 1.;
    }
    else
    {
        return exp(-lambda * y);
    }
}

double MainWindow::funcRandom(double y)
{
    if (y <= (-c / 2.))
    {
        return sqrt(2 * -c) * (sqrt(y) - sqrt(-c / 2.));
    }
    else
    {
        return -1. / lambda * log(1. - c * lambda / 2. - lambda * y);
    }
}

