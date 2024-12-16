#pragma once
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <QtCore/QString>
#include <QtCore/QMetaObject>
#include <QtCore/QMargins>
#include <QtCore/QObject>
#include <QtGui/QPainter>
#include <iostream>
#include <string>


class Chart : public QWidget {
Q_OBJECT
public:
    Chart(std::string title, int maxPoints, QColor color, QWidget *parent = nullptr): QWidget(parent), maxPoints(maxPoints), color(color) {
        layout = new QVBoxLayout(this);

        // QChart と QLineSeries の作成
        chart = new QChart();
        series = new QLineSeries();
        chart->addSeries(series);
        chart->createDefaultAxes();
        chart->setTitle(QString::fromStdString(title));
        chart->legend()->hide();
		chart->setMargins(QMargins(0,0,0,0));

        // QChartView を設定してレイアウトに追加
        auto chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		layout->setSpacing(0);
		layout->setContentsMargins(0,0,0,0);
        layout->addWidget(chartView);
    }
	
    void update(int data) {
		QMetaObject::invokeMethod(this, "updateData", Qt::QueuedConnection, Q_ARG(int, data));
    }
	
public slots:
	void updateData(int data) {
		getSL(1);
		if (series->count() >= maxPoints) series->remove(0);
		getSL(2);
		series->append(series->count(), data);
		getSL(3);
		chart->removeSeries(series);
		getSL(4);
		chart->addSeries(series);
		getSL(5);
		chart->createDefaultAxes();
		getSL(6);
	}

private:
    QChart *chart;
    QLineSeries *series;
    QVBoxLayout *layout;
    int maxPoints;
    QColor color;
};

struct Charts {
    Chart *CPU, *memory, *disk, *wifi, *GPU, *GPU_memory, *battery;

    constexpr static int for_end = 6;
    constexpr static std::string getName(int id) {
        switch(id) {
            case 0: return "CPU";
            case 1: return "memory";
            case 2: return "GPU";
            case 3: return "GPU_memory";
            case 4: return "wifi";
            case 5: return "battery";
            case 6: return "disk";
            default: return "ERROR";
        }
    }
	
	void operator()(std::string name, int v) {
		if (name=="CPU") CPU->update(v);
		else if (name=="memory") memory->update(v);
		else if (name=="GPU") GPU->update(v);
		else if (name=="GPU_memory") GPU_memory->update(v);
		else if (name=="wifi") wifi->update(v);
		else if (name=="battery") battery->update(v);
		else if (name=="disk") disk->update(v);
		else std::cerr << "Charts.operator()の第一引数が間違っています\n";
	}
	void operator()(int id, int v) {
		operator()(getName(id),v);
	}
	void operator()(int v) {
		for (int i = 0; i <= for_end; i++) operator()(i,v);
	}
};
