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
    void putData();
    void startGenerating();
    void stopGenerating();
    void simulateButtonClick();
    void updateGenerationRate(); // Novo slot para atualizar a taxa de geração com base na posição do slider
    void connectToServer(); // Novo slot para conectar ao servidor
    void disconnectFromServer(); // Novo slot para desconectar do servidor

private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QTimer clickTimer;
};

#endif // MAINWINDOW_H
