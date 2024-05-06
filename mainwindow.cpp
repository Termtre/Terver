#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>
#include <QtCharts/QtCharts>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Фомичев Дмитрий 3821Б1ПМмм1");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    table.clear();

    ui->tablewidget->setRowCount(N);

    for (int i = 0; i < N; i++)
    {
        double x = funcRandom(static_cast<double>(rand()) / RAND_MAX);
        table.push_back(x);
        ui->tablewidget->setItem(i, 0, new QTableWidgetItem((QString::number(x))));
    }

    std::sort(table.begin(), table.end());

    double theorMathExp = c * c / 12. + c + 1.;
    double theorDis = 0.25 * pow((2. + c), 3) - pow(c, 3) / 12. - theorMathExp * theorMathExp;

    double statMathExp = 0.;
    double statDis = 0.;

    double R = table.back() - table.front();
    double Median = N % 2 ? table[N / 2] : (table[N / 2] + table[N / 2 - 1]) / 2.;

    for (int i = 0; i < N; i++)
    {
        ui->tablewidget->setItem(i, 0, new QTableWidgetItem((QString::number(table[i]))));

        statMathExp += table[i];
    }

    statMathExp /= static_cast<double>(N);

    for (int i = 0; i < N; i++)
    {
        statDis += pow((table[i] - statMathExp), 2.);
    }

    statDis /= static_cast<double>(N);

    ui->tableWidget_2->setItem(0, 0, new QTableWidgetItem((QString::number(theorMathExp))));
    ui->tableWidget_2->setItem(0, 1, new QTableWidgetItem((QString::number(statMathExp))));
    ui->tableWidget_2->setItem(0, 2, new QTableWidgetItem((QString::number(abs(theorMathExp - statMathExp)))));

    ui->tableWidget_2->setItem(0, 3, new QTableWidgetItem((QString::number(theorDis))));
    ui->tableWidget_2->setItem(0, 4, new QTableWidgetItem((QString::number(statDis))));
    ui->tableWidget_2->setItem(0, 5, new QTableWidgetItem((QString::number(abs(theorDis - statDis)))));

    ui->tableWidget_2->setItem(0, 6, new QTableWidgetItem((QString::number(Median))));
    ui->tableWidget_2->setItem(0, 7, new QTableWidgetItem((QString::number(R))));

    graphics();
    gistogramma();
}

void MainWindow::on_changeC_editingFinished()
{
    bool ok;
    ui->changeC->text().toDouble(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start->setEnabled(false);
        changeCTrue = false;
        return;
    }

    if (ui->changeC->text().toDouble() > 0. || ui->changeC->text().toDouble() < -2.)
    {
        QMessageBox msgBox;
        msgBox.setText("Параметр C не может быть больше нуля или меньше -2");
        msgBox.exec();
        ui->start->setEnabled(false);
        changeCTrue = false;
        return;
    }

    changeCTrue = true;
    if (changeCTrue && changeNTrue && changeN2True) ui->start->setEnabled(true);
    c = ui->changeC->text().toDouble();
    if (c != -2.) lambda = 2. / (2. + c);
}


void MainWindow::on_changeN_editingFinished()
{
    bool ok;
    ui->changeN->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start->setEnabled(false);
        changeNTrue = false;

        return;
    }

    if (ui->changeN->text().toInt() < 1)
    {
        QMessageBox msgBox;
        msgBox.setText("Количество не может быть меньше 1");
        msgBox.exec();
        ui->start->setEnabled(false);
        changeNTrue = false;
        return;
    }

    changeNTrue = true;
    if (changeCTrue && changeNTrue && changeN2True) ui->start->setEnabled(true);
    N = ui->changeN->text().toInt();
}

double MainWindow::funcRandom(double y)
{
    if (y == 0.) return 0.;
    else if (0 < y && y <= (-c / 2.))
    {
        return sqrt(-2 * c * y) + c;
    }
    else
    {
        if (y > 0.9999999999) y = 0.9999999999;
        return -1. / lambda * log(1. - c * lambda / 2. - lambda * y);
    }
}

double MainWindow::theorFunc(double x)
{
    if (x <= c) return 0.;
    else if (c < x && x <= 0.)
    {
        return - x * x / 2. / c + x - c / 2.;
    }
    else
    {
        return c != -2. ? (-exp(-lambda * x) / lambda + 1. / lambda - c / 2.) : -c / 2.;
    }
}

double MainWindow::statFunc(double x)
{
    double res = 0.;
    for (auto it = table.begin(); it != table.end(); ++it)
    {
        if (*it < x)
        {
            res += 1.;
        }
        else
        {
            break;
        }
    }

    return res / static_cast<double>(N);
}

double MainWindow::theorPlot(double x)
{
    if (x <= c) return 0.;
    else if (c <= x && x <= 0.)
    {
        return -x / c + 1.;
    }
    else
    {
        return exp(-lambda * x);
    }
}

void MainWindow::gistogramma()
{
    double a = table[0];
    double b = table[N - 1];
    double h = (b - a) / static_cast<double>(N2);
    double ni = 0.;
    double delta;
    double array[N2][2];
    double heights[N2];

    auto chart = new QChart();
    chart->legend()->setVisible(false);
    chart->setTitle("Гистограмма");

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    auto axisX = new QValueAxis();
    axisX->setLabelFormat("%f");
    axisX->setTickCount(5);
    axisX->setTitleText("Промежутки");
    chart->addAxis(axisX, Qt::AlignBottom);

    auto axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(5);
    axisY->setTitleText("Число элементов");
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->gistogramm->setRenderHint(QPainter::Antialiasing);

    auto top = new QLineSeries();
    auto floor = new QLineSeries();

    int k = 0;

    while (a < b)
    {
        array[k][0] = a;
        a += h;
        array[k++][1] = a;
    }

    ui->tableWidget_3->setRowCount(N2);

    for (int i = 0; i < N2; ++i)
    {
        ui->tableWidget_3->setItem(i, 0, new QTableWidgetItem(QString::number(array[i][1] / 2. + array[i][0] / 2.)));
        ui->tableWidget_3->setItem(i, 1, new QTableWidgetItem(QString::number(theorPlot(array[i][1] / 2. + array[i][0] / 2.))));
    }

    k = 0;

    ni = 1.;

    for (auto it = ++table.begin(); it != table.end(); ++it)
    {
        if (*it <= array[k][1])
        {
            ni += 1.;
        }
        else
        {
            heights[k] = ni / static_cast<double>(N) / h;
            k++;
            ni = 1.;
        }
    }

    for (int i = 0; i < N2; i++)
    {
        *top << QPointF(array[i][0], heights[i]) << QPointF(array[i][1], heights[i]);
        *floor << QPointF(array[i][0], 0.) << QPointF(array[i][1], 0.);
    }

    auto areaPlot = new QAreaSeries(top, floor);

    chart->addSeries(areaPlot);
    areaPlot->attachAxis(axisX);
    areaPlot->attachAxis(axisY);

    axisX->setRange(table.front(), table.back());
    axisY->setRange(0., 10.);

    ui->gistogramm->setChart(chart);
}

void MainWindow::graphics()
{
    auto chart = new QChart();
    chart->legend()->setVisible(true);
    chart->setTitle("Функции распределения");

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    auto axisX = new QValueAxis();
    axisX->setLabelFormat("%f");
    axisX->setTickCount(5);
    axisX->setTitleText("y");
    chart->addAxis(axisX, Qt::AlignBottom);

    auto axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(5);
    axisY->setTitleText("Функции распределения");
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);

    auto theorFuncSeries = new QLineSeries();
    theorFuncSeries->setName("Теоретическая Fη");
    auto statFuncSeries = new QLineSeries();
    statFuncSeries->setName("Выборочная Fη");

    double D = 0.;
    double x, y1, y2, y3;

    for (auto i = table.front(); i <= table.back(); i += 0.001)
    {
        *theorFuncSeries << QPointF(i, theorFunc(i));
    }



    for (int i = 0; i < N; i++)
    {
        x = table[i];
        y1 = theorFunc(x);
        y2 = statFunc(x);
        if (i != 0) y3 = statFunc(table[i - 1]);

        D = abs(y2 - y1) > D ? abs(y2 - y1) : D;

        if (i != 0)
        {
            *statFuncSeries << QPointF(table[i - 1], y2) << QPointF(x, y2);
        }
        else
        {
            *statFuncSeries << QPointF(x, y2);
        }
    }

    ui->calculateD->setText(QString::number(D));

    chart->addSeries(theorFuncSeries);
    theorFuncSeries->attachAxis(axisX);
    theorFuncSeries->attachAxis(axisY);

    chart->addSeries(statFuncSeries);
    statFuncSeries->attachAxis(axisX);
    statFuncSeries->attachAxis(axisY);

    //axisX->setRange(-3., 3.);
    axisY->setRange(0., 1.);

    ui->graphicsView_2->setChart(chart);
}

void MainWindow::on_start2_clicked()
{
    gistogramma();
}


void MainWindow::on_changeN2_editingFinished()
{
    bool ok;
    ui->changeN2->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        ui->start2->setEnabled(false);
        changeN2True = false;

        return;
    }

    if (ui->changeN2->text().toInt() < 1)
    {
        QMessageBox msgBox;
        msgBox.setText("Количество не может быть меньше 1");
        msgBox.exec();
        ui->start2->setEnabled(false);
        changeN2True = false;
        return;
    }

    changeN2True = true;
    if (changeCTrue && changeNTrue && changeN2True) ui->start2->setEnabled(true);
    N2 = ui->changeN2->text().toInt();
}

