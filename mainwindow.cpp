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

    QUrl url("qrc:/about/about.html");

    ui->textBrowser->setSource(url);
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
    divIntervals();
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
    if (0 <= y && y <= (-c / 2.))
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
        if (*it < x + 0.0000001)
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
    if (!table.size()) return;
    ui->tableWidget_3->setRowCount(N2);

    auto chart = new QChart();
    chart->legend()->setVisible(false);
    chart->setTitle("Гистограмма");

    chart->layout()->setContentsMargins(0, 0, 0, 0);
    chart->setBackgroundRoundness(0);

    auto axisX = new QValueAxis();
    axisX->setLabelFormat("%f");
    axisX->setTickCount(5);
    chart->addAxis(axisX, Qt::AlignBottom);

    auto axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(5);
    chart->addAxis(axisY, Qt::AlignLeft);

    //ui->gistogramm->setRenderHint(QPainter::Antialiasing);

    auto top = new QLineSeries();
    auto floor = new QLineSeries();

    double a = table[0];
    double b = table[N - 1];
    double h = (b - a) / static_cast<double>(N2);
    double ni = 0.;
    double delta;
    double middle;
    double maxEps = 0.;

    if (N == 1)
    {
        *top << QPointF(a, 100.);
        *floor << QPointF(a, 0.);
        ui->tableWidget_3->setItem(0, 2, new QTableWidgetItem(QString::number(a / 2.)));
        ui->tableWidget_3->setItem(0, 3, new QTableWidgetItem(QString::number(theorPlot(a / 2.))));
        ui->tableWidget_3->setItem(0, 4, new QTableWidgetItem(QString::number(1)));
        ui->maxProm->setText("0");
    }
    else
    {
        int k = 0;
        for (auto it = table.begin(); a < (b - 0.0000001); a += h)
        {
            for (; it != table.end(); ++it)
            {
                if (*it < (a + h + 0.0000001))
                {
                    ni += 1.;
                }
                else
                {
                    break;
                }
            }

            delta = ni / (static_cast<double>(N) * h);

            *floor << QPointF(a, 0.) << QPointF(a + h, 0.);
            *top << QPointF(a, delta) << QPointF(a + h, delta);

            middle = a + h / 2.;
            ui->tableWidget_3->setItem(k, 0, new QTableWidgetItem("(" + QString::number(a) + ", " + QString::number(a + h) + ")"));
            ui->tableWidget_3->setItem(k, 1, new QTableWidgetItem(QString::number(ni)));
            ui->tableWidget_3->setItem(k, 2, new QTableWidgetItem(QString::number(middle)));
            ui->tableWidget_3->setItem(k, 3, new QTableWidgetItem(QString::number(theorPlot(middle))));
            ui->tableWidget_3->setItem(k++, 4, new QTableWidgetItem(QString::number(delta)));
            maxEps = std::max(maxEps, abs(delta - theorPlot(middle)));

            ni = 0.;
        }
    }

    ui->maxProm->setText(QString::number(maxEps));

    ui->tableWidget_3->verticalHeader()->hide();

    auto areaPlot = new QAreaSeries(top, floor);

    chart->addSeries(areaPlot);
    areaPlot->attachAxis(axisX);
    areaPlot->attachAxis(axisY);

    axisX->setRange(table.front(), table.back());
    //axisY->setRange(0., 10.);

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
    axisX->setTickCount(7);
    axisX->setTitleText("y");
    chart->addAxis(axisX, Qt::AlignBottom);

    auto axisY = new QValueAxis();
    axisY->setLabelFormat("%f");
    axisY->setTickCount(10);
    axisY->setTitleText("Функции распределения");
    chart->addAxis(axisY, Qt::AlignLeft);

    ui->graphicsView_2->setRenderHint(QPainter::Antialiasing);

    auto theorFuncSeries = new QLineSeries();
    theorFuncSeries->setName("Теоретическая Fη");
    auto statFuncSeries = new QLineSeries();
    statFuncSeries->setName("Выборочная Fη");

    double D = 0.;
    double x, y1, y2;

    for (double i = c; i <= 2.; i += 0.001)
    {
        *theorFuncSeries << QPointF(i, theorFunc(i));
    }

    int p = N / 10000;

    for (int i = 0; i < N; i++)
    {
        x = table[i];
        y1 = theorFunc(x);
        y2 = statFunc(x);

        D = abs(y2 - y1) > D ? abs(y2 - y1) : D;

        if (N <= 10000 || !(i % p))
        {
            if (i != 0)
            {
                *statFuncSeries << QPointF(table[i - 1], y2) << QPointF(x, y2);
            }
            else
            {
                *statFuncSeries << QPointF(x, 0.) << QPointF(x, y2);
            }
        }
    }

    *statFuncSeries << (QPointF(2., y2));

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

void MainWindow::divIntervals()
{
    if (!table.size()) return;

    ui->tableWidget->setRowCount(numberIntervals + 1);

    double b = table.back();
    double a = table.front();
    double h = (b - a) / static_cast<double>(numberIntervals - 2);
    double q;
    double ni = 0.;
    int numberOfFirstElem = 0;

    double testR0 = 0.;

    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(-INFINITY)));

    for (auto it = table.begin(); it != table.end(); ++it)
    {
        if (*it != table.front()) break;
        else numberOfFirstElem++;
    }

    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("(" + QString::number(-INFINITY) + ", " + QString::number(a) + "]"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem(QString::number(numberOfFirstElem)));
    ui->tableWidget->setItem(1, 2, new QTableWidgetItem(QString::number(a)));
    ui->tableWidget->setItem(1, 3, new QTableWidgetItem(QString::number(getQ(a, -INFINITY))));

    double temp = static_cast<double>(numberOfFirstElem) - static_cast<double>(N) * getQ(a, -INFINITY);
    testR0 += (temp * temp) / (static_cast<double>(N) * getQ(a, -INFINITY));

    ui->tableWidget->setItem(numberIntervals, 0, new QTableWidgetItem("(" + QString::number(b) + ", " + QString::number(INFINITY) + ")"));
    ui->tableWidget->setItem(numberIntervals, 1, new QTableWidgetItem(QString::number(0)));
    ui->tableWidget->setItem(numberIntervals, 3, new QTableWidgetItem(QString::number(getQ(INFINITY, b))));
    ui->tableWidget->setItem(numberIntervals, 2, new QTableWidgetItem(QString::number(INFINITY)));

    temp = static_cast<double>(N) * getQ(INFINITY, b);
    testR0 += (temp * temp) / (static_cast<double>(N) * getQ(INFINITY, b));

    double testForTest = getQ(INFINITY, b) + getQ(a, -INFINITY);

    QStringList header;
    for (int i = 0; i <= numberIntervals; i++)
    {
        header << QString::number(i);
    }

    int k = 2;
    for (auto it = ++table.begin(); a < (b - 0.0000001); a += h)
    {
        for (; it != table.end(); ++it)
        {
            if (*it < (a + h + 0.0000001))
            {
                ni += 1.;
            }
            else
            {
                break;
            }
        }

        q = getQ(a + h, a);

        temp = ni - static_cast<double>(N) * q;
        testR0 += temp * temp / (static_cast<double>(N) * q);

        testForTest += q;

        ui->tableWidget->setItem(k, 0, new QTableWidgetItem("(" + QString::number(a) + ", " + QString::number(a + h) + "]"));
        ui->tableWidget->setItem(k, 1, new QTableWidgetItem(QString::number(ni)));
        ui->tableWidget->setItem(k, 2, new QTableWidgetItem(QString::number(a + h)));
        ui->tableWidget->setItem(k++, 3, new QTableWidgetItem(QString::number(q)));
        ni = 0.;
    }

    ui->tableWidget->setVerticalHeaderLabels(header);

    ui->FR0edit->setText(QString::number(testR0));
    ui->hypStatus->setText(QString::number(testForTest));
}

double MainWindow::integral_trapezoid(double b, double a)
{
    if (b == INFINITY) return 1. - theorFunc(a);
    if (a == -INFINITY) return theorFunc(b);

    double h = b / 2. - a / 2.;
    double g1, g2;

    g1 = theorPlot(b);
    g2 = theorPlot(a);

    return h * (g1 + g2);

}

double MainWindow::getQ(double b, double a)
{
    if (b == INFINITY)
    {
        return 1. - theorFunc(a);
    }

    if (a == -INFINITY)
    {
        return theorFunc(b);
    }

    return theorFunc(b) - theorFunc(a);
}

double MainWindow::FXi(double x)
{
    double p = (static_cast<double>(numberIntervals - 1)) / 2.;

    if (p < 1)
    {}
    else
    {}
}

double MainWindow::I1(double x, double p)
{

}

double MainWindow::I2(double x, double p)
{

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


void MainWindow::on_numberIntervalsEdit_editingFinished()
{
    bool ok;
    ui->numberIntervalsEdit->text().toInt(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        return;
    }

    if (ui->numberIntervalsEdit->text().toInt() < 1)
    {
        QMessageBox msgBox;
        msgBox.setText("Количество интервалов не может быть меньше 1");
        msgBox.exec();

        return;
    }

    numberIntervals = ui->numberIntervalsEdit->text().toInt();
}


void MainWindow::on_pushButton_clicked()
{
    divIntervals();
}

