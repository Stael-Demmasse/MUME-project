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
    void on_startButton_clicked(bool checked);
    void on_pitchShiftButton_clicked(bool checked);
    void on_echoButton_clicked(bool checked);
    void on_robotButton_clicked(bool checked);

    void on_shiftValue_sliderMoved(int position);


private:
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
