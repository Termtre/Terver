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

    ui->tableWidget->setColumnWidth(0, 180);
    ui->tableWidget->setColumnWidth(1, 50);
    ui->tableWidget->setColumnWidth(2, 100);
    ui->tableWidget->setColumnWidth(3, 170);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_start_clicked()
{
    table.clear();

    ui->tablewidget->setRowCount(N);

    table.resize(N);
    generateRandomSeq(table);

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

    double b = -INFINITY;
    double a = 0.;
    double q = 0.;
    double ni = 0.;

    ui->tableWidget->setItem(0, 2, new QTableWidgetItem(QString::number(-INFINITY)));

    QStringList header;
    for (int i = 0; i <= numberIntervals; i++)
    {
        header << QString::number(i);
    }

    double R0 = 0.;
    double temp;

    for (int i = 1, j = 0; i <= numberIntervals; i++)
    {
        a = b;
        if (i != numberIntervals)
            b = funcRandom((static_cast<double>(i) / static_cast<double>(numberIntervals)));
        else b = INFINITY;

        if (i == 1)
        {
            ui->tableWidget->setItem(1, 0,
                                     new QTableWidgetItem("(" + QString::number(-INFINITY) + ", " + QString::number(b) + "]"));
        }
        else if (i == numberIntervals)
        {
            ui->tableWidget->setItem(numberIntervals, 0,
                                     new QTableWidgetItem("(" + QString::number(a) + ", " + QString::number(INFINITY) + ")"));
        }
        else
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem("(" + QString::number(a) + ", " + QString::number(b) + "]"));
        }

        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(b)));

        for (; j < N; j++)
        {
            if (table[j] < (b + 0.0000001))
            {
                ni += 1.;
            }
            else
            {
                break;
            }
        }

        q = getQ(b, a);

        temp = ni - static_cast<double>(N) * q;
        R0 += temp * temp / q / static_cast<double>(N);

        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(ni)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(q)));
        ni = 0.;
    }

    double FR0 = FXi(R0, numberIntervals - 1);

    ui->FR0edit->setText(QString::number(FR0));

    ui->tableWidget->setVerticalHeaderLabels(header);

    if (FR0 >= alpha)
    {
        ui->hypStatus->setText("принимается");
    }
    else
    {
        ui->hypStatus->setText("отвергается");
    }

    testHyp();
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

double MainWindow::fXi(double x, double r)
{
    if (x <= 0.)
    {
        return 0.;
    }
    else
    {
        double temp = r / 2.;
        return pow(2., -temp) / std::tgamma(temp) * pow(x, temp - 1.) * exp(-x / 2.);
    }
}

double MainWindow::FXi(double x, double r)
{
    double result = 0.;
    double a = 0.;
    double b = x;
    int n = 10000;

    for (int i = 1; i <= n; i++)
    {
        result += fXi(a + (b - a) * static_cast<double>(i - 1) / static_cast<double>(n), r)
                  + fXi(a + (b - a) * static_cast<double>(i) / static_cast<double>(n), r);
    }

    return 1. - result * (b - a) / 2. / static_cast<double>(n);
}

double MainWindow::I1(double x, double p)
{
    double result = exp(-x) * pow(x, p) / std::tgamma(p + 1);
    double temp = 1.;
    double del = 1.;

    for (int r = 1; r <= 20; r++)
    {
        del *= (p + static_cast<double>(r));
        temp += pow(x, r) / del;
    }

    return result * temp;
}

double MainWindow::I2(double x, double p)
{
    double result = exp(-x) * pow(x, p) / std::tgamma(p);
    double del = 1. + 20.;

    for (int i = 20; i > 0; i--)
    {
        del = x + (static_cast<double>(i) - p) / del;
    }

    return 1. - result / del;
}

void MainWindow::testHyp()
{
    int count = 0;

    std::vector<double> vec;
    double R0;
    double a;
    double b;
    double ni;
    double q;
    double temp;
    double FR0;

    std::vector<double> intervals(numberIntervals + 1);
    intervals[0] = -INFINITY;
    intervals[numberIntervals] = INFINITY;

    int Count = 1;

    for (auto it = ++intervals.begin(); it != --intervals.end(); ++it)
    {
        *it = funcRandom((static_cast<double>(Count++) / static_cast<double>(numberIntervals)));
    }

    for (int i = 0; i < 100; i++)
    {   
        vec.resize(N);

        generateRandomSeq(vec);

        R0 = 0.;
        a = 0.;
        b = 0.;
        ni = 0.;
        q = 0.;

        for (int i = 1, j = 0; i <= numberIntervals; i++)
        {
            for (; j < N; j++)
            {
                if (vec[j] < (intervals[i] + 0.0000001))
                {
                    ni += 1.;
                }
                else
                {
                    break;
                }
            }

            q = getQ(intervals[i], intervals[i - 1]);

            temp = ni - static_cast<double>(N) * q;
            R0 += temp * temp / q / static_cast<double>(N);

            ni = 0.;
        }

        FR0 = FXi(R0, static_cast<double>(numberIntervals - 1));

        if (FR0 >= alpha)
        {
            count++;
        }
    }

    ui->tableWidget_4->setItem(0, 0, new QTableWidgetItem(QString::number(count)));
    ui->tableWidget_4->setItem(0, 1, new QTableWidgetItem(QString::number(100 - count)));
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


void MainWindow::on_lineEdit_2_editingFinished()
{
    bool ok;
    double temp = ui->lineEdit_2->text().toDouble(&ok);

    if (ok == false)
    {
        QMessageBox msgBox;
        msgBox.setText("Недопустимые символы");
        msgBox.exec();

        return;
    }

    if (temp < 0. || temp > 1.)
    {
        QMessageBox msgBox;
        msgBox.setText("Уровень значимости не может быть меньше нуля или больше единицы");
        msgBox.exec();

        return;
    }

    alpha = temp;
}

void MainWindow::generateRandomSeq(std::vector<double> & vec)
{
    double x;
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        x = funcRandom(static_cast<double>(rand()) / RAND_MAX);
        *it = x;
    }

    std::sort(vec.begin(), vec.end());
}

