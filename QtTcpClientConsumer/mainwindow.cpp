#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isRunning(false) // Inicializa como falso, indicando que o widget não está em execução
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    // Definindo o valor mínimo como 1 e o valor inicial como 1 para o QSlider horizontalSlider
    ui->horizontalSlider->setMinimum(1);
    ui->horizontalSlider->setValue(1);

    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::connectButtonClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::disconnectButtonClicked);
    connect(ui->pushButtonGet, &QPushButton::clicked, this, &MainWindow::getData);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::updateLabel);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::updateList); // Conexão do botão Update ao novo slot

    timer.setSingleShot(false);

    // Inicializa o vetor values com 30 elementos, todos zerados
    values.fill(0, 30);
}

void MainWindow::connectButtonClicked() {
    tcpConnect();
}

void MainWindow::disconnectButtonClicked() {
    socket->disconnectFromHost();
    ui->listWidget->clear(); // Limpa o QListWidget ao desconectar
}

void MainWindow::tcpConnect() {
    QString ipAddress = ui->textEdit->toPlainText();
    if (!ui->listWidget->findItems(ipAddress, Qt::MatchExactly).isEmpty()) {
        qDebug() << "This IP address is already in the list.";
        return; // Não conecta novamente se o endereço IP já estiver na lista
    }

    socket->connectToHost(ipAddress, 1234);
    if(socket->waitForConnected(3000)) {
        qDebug() << "Connected";
        // Adiciona o endereço IP da máquina à QListWidget
        ui->listWidget->addItem(ipAddress);
    } else {
        qDebug() << "Disconnected";
    }
}

void MainWindow::getData() {
    if (isRunning) {
        QString str;
        QByteArray array;
        QStringList list;
        qint64 thetime;
        qDebug() << "to get data...";
        if (socket->state() == QAbstractSocket::ConnectedState) {
            if (socket->isOpen()) {
                qDebug() << "reading...";
                socket->write("get 192.168.1.10 1\r\n");
                socket->waitForBytesWritten();
                socket->waitForReadyRead();
                qDebug() << socket->bytesAvailable();
                while (socket->bytesAvailable()) {
                    str = socket->readLine().replace("\n", "").replace("\r", "");
                    list = str.split(" ");
                    if (list.size() == 2) {
                        bool ok;
                        str = list.at(0);
                        thetime = str.toLongLong(&ok);
                        str = list.at(1);
                        qDebug() << thetime << ": " << str;

                        // Atualiza o vetor values
                        values.push_front(str.toDouble());
                        values.pop_back();

                        // Atualiza o Plotter com os novos dados
                        ui->widget->setData(values);
                    }
                }
            }
        }
    }
}

void MainWindow::onStartClicked() {
    isRunning = true; // Define o widget como em execução
    int interval = ui->horizontalSlider->value() * 1000; // Convertendo para milissegundos
    timer.start(interval);
    connect(&timer, &QTimer::timeout, this, &MainWindow::onTimeout);
}

void MainWindow::onStopClicked() {
    isRunning = false; // Define o widget como parado
    timer.stop();
    disconnect(&timer, &QTimer::timeout, this, &MainWindow::onTimeout);
}

void MainWindow::onTimeout() {
    if (isRunning) { // Verifica se o widget está em execução
        ui->pushButtonGet->click();
    }
}

void MainWindow::updateLabel(int value) {
    ui->label_2->setText(QString::number(value));

    // Se o temporizador estiver em execução, reinicie-o com o novo intervalo
    if (isRunning && timer.isActive()) { // Verifica se o widget está em execução
        int interval = value * 1000; // Convertendo para milissegundos
        timer.start(interval);
    }
}

void MainWindow::updateList() {
    // Armazena os endereços IP das máquinas conectadas
    QStringList connectedMachines;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        connectedMachines << ui->listWidget->item(i)->text();
    }

    // Limpa o QListWidget
    ui->listWidget->clear();

    // Adiciona uma mensagem temporária
    ui->listWidget->addItem("Updating...");

    // Inicia um temporizador para restaurar a lista após 1 segundo
    QTimer::singleShot(1000, [this, connectedMachines]() {
        // Remove a mensagem temporária
        delete ui->listWidget->item(0);

        // Adiciona as máquinas conectadas de volta ao QListWidget
        for (const QString& machine : connectedMachines) {
            ui->listWidget->addItem(machine);
        }
    });
}

MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}
