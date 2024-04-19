#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "worker.h"
#include <QMainWindow>
#include <QMap>
#include <QSettings>
#include <QThread>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow();

  private slots:
    void main_slot_enableAllButtons( bool abValue );
    void on_hexFileButton_clicked();
    void on_programButton_clicked();
    void on_aboutButton_clicked();
    void on_pk2RadioButton_clicked();
    void on_appRadioButton_clicked();
    void main_slot_processOutput( QString asCommandOutput );
    void main_slot_pickitInfo( QString asPicKitInfo );
    void on_detectButton_clicked();
    void on_eraseButton_clicked();
    void on_blankCheckButton_clicked();
    void on_verifyButton_clicked();
    void on_readButton_clicked();
    void on_detectPICkitButton_clicked();
    void on_setNewIDButton_clicked();
    void main_slot_prepareCommandExecution();
    void main_slot_taskCompleted( bool abExitStatus, QString asExitString );

  signals:
    void main_signal_executeCommand( QStringList aobArguments );
    void main_signal_pickitInfo();


  private:
    Ui::MainWindow *ui;
    QString gsHexFileName;
    QThread *workerThread;
    QStringList gobArguments;
    Worker *gobWorker;
    QString programVersion;
    QString activeProgrammer = "PICkit2";
    QMap< QString, QMap< QString, QStringList > > progCmds;
    QSettings *settings;
    void setupProgrammer();
    void selectProgrammer( QString programmer );
};

#endif // MAINWINDOW_H
