#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#pragma execution_character_set("utf-8")
namespace Ui {
class MainWindow;
}

class QStackedWidget;
class VoiceIdentification_win;
class VoiceLib_win;
class VoiceRegistration_win;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStackedWidget *m_main_widget;
    VoiceIdentification_win *m_voiceIdentification_win;
    VoiceLib_win *m_voiceLib_win;
    VoiceRegistration_win *m_voiceRegistration_win;

private slots:
    void switchToVoiceIdentificationWin();
    void switchToVoiceLibWin();
    void switchToVoiceRegistrationWin();
    void exit();
};

#endif // MAINWINDOW_H
