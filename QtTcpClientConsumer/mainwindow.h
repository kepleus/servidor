#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void tcpConnect();

public slots:
    void getData();
    void connectButtonClicked();
    void disconnectButtonClicked();
    void onStartClicked(); // Slot para iniciar o widget
    void onStopClicked(); // Slot para parar o widget
    void onTimeout();
    void updateLabel(int value);
    void updateList(); // slot para atualizar a lista de máquinas conectadas

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer timer;
    bool isRunning; // Variável para controlar o estado de execução do widget
    QVector<double> values; // Vetor para armazenar os valores recebidos do servidor
};

#endif // MAINWINDOW_H

