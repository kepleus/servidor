#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    socket(new QTcpSocket(this))
{
    ui->setupUi(this);

    // Definindo o valor mínimo como 1 e o valor inicial como 1 para o QSlider horizontalSlider_3
    ui->horizontalSlider_3->setMinimum(1);
    ui->horizontalSlider_3->setValue(1);

    // Conexões de sinais e slots
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::disconnectFromServer);
    connect(ui->pushButtonPut, &QPushButton::clicked, this, &MainWindow::putData);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::startGenerating);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::stopGenerating);
    connect(ui->horizontalSlider_3, &QSlider::valueChanged, this, &MainWindow::updateGenerationRate);
}

void MainWindow::connectToServer() {
    QString ipAddress = ui->textEdit->toPlainText(); // Obtém o IP do servidor do QTextEdit
    socket->connectToHost(ipAddress, 1234); // Conecta ao servidor usando o IP fornecido e a porta 1234
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
    } else {
        qDebug() << "Disconnected";
    }
}

void MainWindow::disconnectFromServer() {
    socket->disconnectFromHost(); // Desconecta do servidor
}

void MainWindow::putData(){
    QDateTime datetime;
    QString str;
    qint64 msecdate;

    int minValue = ui->lcdNumber_2->value(); // Obtém o valor mínimo do LCD
    int maxValue = ui->lcdNumber->value(); // Obtém o valor máximo do LCD

    if(socket->state() == QAbstractSocket::ConnectedState){
        msecdate = QDateTime::currentDateTime().toMSecsSinceEpoch();
        int generatedValue = rand() % (maxValue - minValue + 1) + minValue; // Gera um valor dentro do intervalo
        str = "set " + QString::number(msecdate) + " " +
              QString::number(generatedValue) + "\r\n";

        ui->textBrowser->append(str); // Adiciona o valor ao textBrowser

        qDebug() << str;
        qDebug() << socket->write(str.toStdString().c_str())
                 << " bytes written";
        if(socket->waitForBytesWritten(3000)){
            qDebug() << "wrote";
        }
    }
}

void MainWindow::startGenerating(){
    int interval = ui->horizontalSlider_3->value(); // Obtém o valor do slider
    clickTimer.start(interval * 1000); // Define o intervalo desejado aqui (em milissegundos)
    connect(&clickTimer, &QTimer::timeout, this, &MainWindow::simulateButtonClick);
}

void MainWindow::stopGenerating(){
    clickTimer.stop();
    disconnect(&clickTimer, &QTimer::timeout, this, &MainWindow::simulateButtonClick);
}

void MainWindow::simulateButtonClick(){
    ui->pushButtonPut->click();
}

void MainWindow::updateGenerationRate(){
    int interval = ui->horizontalSlider_3->value(); // Obtém o valor do slider
    clickTimer.setInterval(interval * 1000); // Atualiza o intervalo do temporizador
}

MainWindow::~MainWindow(){
    delete socket;
    delete ui;
}
