#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_CreateGraphics_clicked();

    void on_setC_editingFinished();

private:
    QStringList tableColumn;
    QChart* chart = nullptr;
    QChart* chart2 = nullptr;
    QLineSeries* rasprV = nullptr;
    QLineSeries* funcV = nullptr;
    QValueAxis* axisX = nullptr;
    QValueAxis* axisY = nullptr;
    QValueAxis* axisX2 = nullptr;
    QValueAxis* axisY2 = nullptr;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
