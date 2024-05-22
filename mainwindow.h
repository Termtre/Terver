#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_start_clicked();

    void on_changeC_editingFinished();

    void on_changeN_editingFinished();

    void on_start2_clicked();

    void on_changeN2_editingFinished();

    void on_numberIntervalsEdit_editingFinished();

    void on_pushButton_clicked();

    void on_lineEdit_2_editingFinished();

private:
    bool changeCTrue = true;
    bool changeNTrue = true;
    bool changeN2True = true;

    double c = -1.;
    double lambda = 2.;
    int N = 10;
    int N2 = 10;
    int numberIntervals = 10;
    double alpha = 0.1;

    std::vector<double> table;


    void generateRandomSeq(std::vector<double>&);
    double funcRandom(double y);
    double theorFunc(double x);
    double statFunc(double x);
    double theorPlot(double x);
    void gistogramma();
    void graphics();
    void divIntervals();
    double integral_trapezoid(double b, double a);
    double getQ(double b, double a);
    double FXi(double x, double r);
    double fXi(double x, double r);
    double I1(double x, double p);
    double I2(double x, double p);
    void testHyp();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
