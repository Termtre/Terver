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

private:
    bool changeCTrue = true;
    bool changeNTrue = true;
    bool changeN2True = true;

    double c = -1.;
    double lambda = 2.;
    int N = 10;
    int N2 = 10;

    std::vector<double> table;

    double funcRandom(double y);
    double theorFunc(double x);
    double statFunc(double x);
    double theorPlot(double x);
    void gistogramma();
    void graphics();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
