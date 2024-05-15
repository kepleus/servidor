#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QVector>

class Plotter : public QWidget {
    Q_OBJECT

public:
    explicit Plotter(QWidget *parent = nullptr);
    void setData(const QVector<double> &data); // Função para definir os dados

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> m_data; // Vetor para armazenar os dados
};

#endif // PLOTTER_H
