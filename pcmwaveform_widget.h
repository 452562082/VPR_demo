#ifndef PCMWAVEFORM_WIDGET_H
#define PCMWAVEFORM_WIDGET_H

#include <QWidget>
#include <QList>
#include <QSplineSeries>
#include <QScatterSeries>
#include <QChart>
#include <QChartView>

namespace Ui {
class PCMWaveform_widget;
}

using namespace QtCharts;
class PCMWaveform_widget : public QWidget
{
    Q_OBJECT

public:
    explicit PCMWaveform_widget(QWidget *parent = 0);
    ~PCMWaveform_widget();
    void UpdateForm(int value);

private:
    Ui::PCMWaveform_widget *ui;

    int m_max_size;
    int m_max_value;
    QList<double> m_data;
    QChart *m_chart;
    QChartView *m_chartView;
    QSplineSeries *m_splineSeries;
};

#endif // PCMWAVEFORM_WIDGET_H
