#include "plotter.h"
#include <QPainter>
#include <QBrush>
#include <QPen>

Plotter::Plotter(QWidget *parent) : QWidget(parent) {}

void Plotter::setData(const QVector<double> &data) {
    m_data = data; // Define os dados recebidos
    update(); // Chama o repaint
}

void Plotter::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // Ativa o anti-aliasing para suavizar as bordas das formas
    QBrush brush;
    QPen pen;

    brush.setColor(QColor(0, 255, 255));
    brush.setStyle(Qt::SolidPattern);

    pen.setColor(QColor(255, 0, 0));
    pen.setWidth(2);

    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawRect(0, 0, width(), height());
    pen.setColor(QColor(255, 255, 255));

    // Desenha o gráfico com base nos dados recebidos
    if (!m_data.isEmpty()) {
        double stepX = static_cast<double>(width()) / (m_data.size() - 1);
        double maxY = *std::max_element(m_data.constBegin(), m_data.constEnd());
        double stepY = maxY > 0 ? static_cast<double>(height()) / maxY : 0;

        QPointF prevPoint(0, height() - m_data.first() * stepY);
        for (int i = 1; i < m_data.size(); ++i) {
            QPointF currentPoint(i * stepX, height() - m_data[i] * stepY);
            painter.drawLine(prevPoint, currentPoint);
            prevPoint = currentPoint;
        }
    }
}
