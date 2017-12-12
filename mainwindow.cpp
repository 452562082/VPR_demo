#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "voiceidentification_win.h"
#include "voicelib_win.h"
#include "voiceregistration_win.h"
#include <QStackedWidget>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_voiceIdentification_win = new VoiceIdentification_win;
    m_voiceLib_win = new VoiceLib_win;
    m_voiceRegistration_win = new VoiceRegistration_win;

    m_main_widget = new QStackedWidget;
    m_main_widget->addWidget(m_voiceIdentification_win);
    m_main_widget->addWidget(m_voiceLib_win);
    m_main_widget->addWidget(m_voiceRegistration_win);

    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(m_main_widget);
    main_layout->setMargin(0);
    ui->main_widget->setLayout(main_layout);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->showFullScreen();

    connect(m_voiceIdentification_win,SIGNAL(switchToVoiceLibWin()),this,SLOT(switchToVoiceLibWin()));
    connect(m_voiceIdentification_win,SIGNAL(switchToVoiceRegistrationWin()),this,SLOT(switchToVoiceRegistrationWin()));
    connect(m_voiceLib_win,SIGNAL(switchToVoiceIdentificationWin()),this,SLOT(switchToVoiceIdentificationWin()));
    connect(m_voiceRegistration_win,SIGNAL(switchToVoiceIdentificationWin()),this,SLOT(switchToVoiceIdentificationWin()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchToVoiceIdentificationWin()
{
    m_main_widget->setCurrentWidget(m_voiceIdentification_win);
    m_voiceIdentification_win->switch_identificationStatus();
}

void MainWindow::switchToVoiceLibWin()
{
    m_voiceLib_win->bindData();
    m_main_widget->setCurrentWidget(m_voiceLib_win);
    m_voiceIdentification_win->switch_identificationStatus();
}

void MainWindow::switchToVoiceRegistrationWin()
{
    m_main_widget->setCurrentWidget(m_voiceRegistration_win);
    m_voiceIdentification_win->switch_identificationStatus();
}
