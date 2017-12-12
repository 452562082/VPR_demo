#include "pcmwaveform_widget.h"
#include "ui_pcmwaveform_widget.h"
#include <QHBoxLayout>

namespace {
    const int peak_value = 5000;
}

PCMWaveform_widget::PCMWaveform_widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PCMWaveform_widget)
{
    ui->setupUi(this);
    m_max_size = 50;
    m_max_value = peak_value + 50;//50为预留绘制间距

    m_splineSeries = new QSplineSeries();
    for (int i = 0; i < m_max_size;++i) {
        m_splineSeries->append(i * 10, 0);
    }
    QPen pen;
    pen.setColor(QColor("#3399ff"));
    pen.setWidth(2);
    m_splineSeries->setPen(pen);
    m_chart = new QChart();
    m_chart->addSeries(m_splineSeries);
    m_chart->legend()->hide();
    m_chart->createDefaultAxes();
    m_chart->axisY()->setRange(-m_max_value, m_max_value);
    m_chart->setBackgroundVisible(false);
    m_chart->axisX()->hide();
    m_chart->axisY()->hide();

    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_chartView);
    setLayout(main_layout);

    //背景透明
    this->setStyleSheet("* {background-color: transparent;}");
}

PCMWaveform_widget::~PCMWaveform_widget()
{
    delete ui;
}

void PCMWaveform_widget::UpdateForm(int value)
{
    if(value > peak_value){
        value = peak_value;
    }else if(value < -1 * peak_value){
        value = -1 * peak_value;
    }

    m_data << value;
    while (m_data.size() > m_max_size) {
        m_data.removeFirst();
    }
    if (isVisible()) {
        int delta = m_max_size - m_data.size();
        for (int i = 0; i < m_data.size(); ++i) {
            m_splineSeries->replace(delta+i, m_splineSeries->at(delta+i).x(), m_data.at(i));
        }
    }
}
