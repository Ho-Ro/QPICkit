#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QLoggingCategory>

/**
  Constructor de la clase MainWindow
  @param *parent - Apuntador al objeto padre.
*/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // suppress nasty warnings, e.g. "qt.qpa.xcb: QXcbConnection: XCB error: 3 (BadWindow) ..."
    QLoggingCategory::setFilterRules( "qt.qpa.xcb=false" );
    ui->setupUi(this);

    programVersion = "v2.2";

    setWindowTitle(QString("PICkit2 %1").arg(programVersion));

    gobWorker = new Worker;
    workerThread = new QThread;
    gobWorker->moveToThread(workerThread);
    connect(gobWorker,SIGNAL(worker_signal_prepareCommandExecution()),this,SLOT(main_slot_prepareCommandExecution()));
    connect(gobWorker,SIGNAL(worker_signal_processOutput(QString)),this,SLOT(main_slot_processOutput(QString)));
    connect(this,SIGNAL(main_signal_pickitInfo()),gobWorker,SLOT(worker_slot_pickitInfo()));
    connect(gobWorker,SIGNAL(worker_signal_pickitInfo(QString)),this,SLOT(main_slot_pickitInfo(QString)));
    connect(this,SIGNAL(main_signal_executeCommand(QStringList)),gobWorker,SLOT(worker_slot_executeCommand(QStringList)));
    connect(gobWorker,SIGNAL(worker_signal_taskCompleted(bool,QString)),this,SLOT(main_slot_taskCompleted(bool,QString)));
    connect(workerThread,SIGNAL(finished()),gobWorker,SLOT(deleteLater()));
    connect(workerThread,SIGNAL(finished()),workerThread,SLOT(deleteLater()));
    workerThread->start();

    QFile styleFile("style.qss");
    if(styleFile.exists() && styleFile.open(QFile::ReadOnly)){
        QString StyleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(StyleSheet);
    }else{
        this->ui->defaultThemeRadioButton->setChecked(true);
        this->ui->customThemeRadioButton->setChecked(false);
        this->ui->customThemeRadioButton->setEnabled(false);
    }
}

/**
  Enables or disables all buttons on the main tab
  to avoid sending unwanted commands
  during the execution of a process.
  @param abValue - true: Enable all buttons,
                   false: Disable all buttons.
*/
void MainWindow::main_slot_enableAllButtons(bool abValue)
{
    ui->detectButton->setEnabled(abValue);
    ui->programButton->setEnabled(abValue);
    ui->verifyButton->setEnabled(abValue);
    ui->eraseButton->setEnabled(abValue);
    ui->readButton->setEnabled(abValue);
    ui->hexFileButton->setEnabled(abValue);
    ui->blankCheckButton->setEnabled(abValue);
}

/**
  Destructor de la clase
*/
MainWindow::~MainWindow()
{
    delete ui;
    workerThread->quit();
    workerThread->wait();
}

/**
  Click event for button "HEX File..."
*/
void MainWindow::on_hexFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Hex"), "", tr("Hex files (*.hex)"));
    ui->hexFileLineEdit->setText(fileName);
}

/**
  Click event for button "Burn"
*/
void MainWindow::on_programButton_clicked()
{
    QDir lobDir(QCoreApplication::applicationDirPath());
    if(ui->hexFileLineEdit->text().isNull() || ui->hexFileLineEdit->text().isEmpty()){
        gsHexFileName = QFileDialog::getOpenFileName(this, tr("Open Hex"), "", tr("Hex files (*.hex)"));
        ui->hexFileLineEdit->setText(gsHexFileName);
    } else {
        gsHexFileName = ui->hexFileLineEdit->text();
    }

    if(!gsHexFileName.isNull() && !gsHexFileName.isEmpty()){
        gsHexFileName = lobDir.relativeFilePath(ui->hexFileLineEdit->text());

        gobArguments << "-p" << "-f" << gsHexFileName << "-m" << "-j";
        emit(main_signal_executeCommand(gobArguments));
    }else{
        QMessageBox::critical(this,"ERROR","Please, select a valid Hex file");
    }
}

/**
  Click event for button "Detect"
*/
void MainWindow::on_detectButton_clicked()
{
    gobArguments << "-p" << "-i";
    emit(main_signal_executeCommand(gobArguments));
}

/**
  Click event for button "Erase""
*/
void MainWindow::on_eraseButton_clicked()
{
    gobArguments << "-p" << "-e";
    emit(main_signal_executeCommand(gobArguments));
}

/**
  Click event for button "Blank Check"
*/
void MainWindow::on_blankCheckButton_clicked()
{
    gobArguments << "-p" << "-c" << "-j";
    emit(main_signal_executeCommand(gobArguments));
}

/**
  Click event for button "Verify"
*/
void MainWindow::on_verifyButton_clicked()
{
    QDir lobDir(QCoreApplication::applicationDirPath());

    if(ui->hexFileLineEdit->text().isNull() || ui->hexFileLineEdit->text().isEmpty()){
        gsHexFileName = QFileDialog::getOpenFileName(this, tr("Open Hex"), "", tr("Hex files (*.hex)"));
        ui->hexFileLineEdit->setText(gsHexFileName);
    } else {
        gsHexFileName = ui->hexFileLineEdit->text();
    }

    if(!gsHexFileName.isNull() && !gsHexFileName.isEmpty()){
        gsHexFileName = lobDir.relativeFilePath(ui->hexFileLineEdit->text());

        gobArguments << "-p" << "-f" << gsHexFileName << "-y" << "-j";
        emit(main_signal_executeCommand(gobArguments));
    }else{
        QMessageBox::critical(this,"ERROR","Please, select a valid Hex file");
    }
}

/**
  Click event for button "Read"
*/
void MainWindow::on_readButton_clicked()
{
    QDir lobDir(QCoreApplication::applicationDirPath());
    gsHexFileName = QFileDialog::getSaveFileName(this, tr("Save Hex"), "", tr("Hex files (*.hex)"));
    if(!gsHexFileName.isNull() && !gsHexFileName.isEmpty()){
        if(!gsHexFileName.contains(".hex")){
            gsHexFileName += ".hex";
        }
        ui->hexFileLineEdit->setText(gsHexFileName);
        gsHexFileName = lobDir.relativeFilePath(ui->hexFileLineEdit->text());

        gobArguments << "-p" << "-gf" << gsHexFileName << "-j";
        emit(main_signal_executeCommand(gobArguments));
    }else{
        QMessageBox::critical(this,"ERROR","Please, select a valid Hex file");
    }
}

/**
  Click event for button "Detect programmer"
*/
void MainWindow::on_detectPICkitButton_clicked()
{
    ui->pickitInfoTextArea->clear();
    emit(main_signal_pickitInfo());
}

/**
  Click event for button "Set New ID"
*/
void MainWindow::on_setNewIDButton_clicked()
{
    gobArguments << "-n" << ui->setNewIDLineEdit->text();
    emit(main_signal_executeCommand(gobArguments));
}

/**
  Prepares the application to execute the following command,
  clearing the event log,
  disabling all buttons on the main tab
  and clearing the argument array.
*/
void MainWindow::main_slot_prepareCommandExecution()
{
    this->main_slot_enableAllButtons(false);
    ui->logTextArea->clear();
    gobArguments.clear();
}

/**
  Slot which is executed when the current command has been
  notified as completed by the Worker class.
  @param abExitStatus - true: The command was executed successfully.
                        false: The command failed.
         asExitString - String containing the text of the error occurred.
*/
void MainWindow::main_slot_taskCompleted(bool abExitStatus, QString asExitString)
{
    this->main_slot_enableAllButtons(true);
    if(!abExitStatus) {
        ui->logTextArea->appendPlainText(asExitString);
    }
}

/**
  Click event for button "About"
*/
void MainWindow::on_aboutButton_clicked()
{
    const char *lsHelpText = ("<h2>QPICkit</h2>"
                "<p>pk2cmd GUI for Linux<br/>"
                "Use with PICkit2 and compatible programmer</p>"
                "<p><a href=\"https://github.com/Ho-Ro/QPICkit\">https://github.com/Ho-Ro/QPICkit</a></p>");

    QMessageBox::about(this, tr("About QPICkit %1").arg(programVersion),
    tr(lsHelpText));
}

/**
  Click event for "Default" theme
*/
void MainWindow::on_defaultThemeRadioButton_clicked()
{
    this->setStyleSheet("");
}

/**
  Click event for "Custom" theme
*/
void MainWindow::on_customThemeRadioButton_clicked()
{
    QFile styleFile("style.qss");
    if(styleFile.exists() && styleFile.open(QFile::ReadOnly)){
        QString StyleSheet = QLatin1String(styleFile.readAll());
        this->setStyleSheet(StyleSheet);
    }
}

/**
  Slot that adds the output of the executed command in the log viewer,
  and in the status bar.
  @param asCommandOutput - String containing the output of the executed command.
*/
void MainWindow::main_slot_processOutput(QString asCommandOutput)
{
    if (!asCommandOutput.isNull() && asCommandOutput != "\n" && asCommandOutput != "\r"
        && asCommandOutput != "\r                              \r") { // ignore white space lines
        if ( asCommandOutput.startsWith("\r") ) { // replace current line
            ui->logTextArea->moveCursor(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
            ui->logTextArea->insertPlainText(asCommandOutput.remove(0, 1)); // remove leading '\r' );
        } else { // append new line
            ui->logTextArea->appendPlainText(asCommandOutput);
        }
        ui->statusBar->showMessage(asCommandOutput.simplified(),5000);
    }
}

/**
  Slot that inserts the information of the currently connected pickit2
  to the log viewer.
  @param asPicKitInfo - String containing the information retrieved
                        from the connected PicKit2.
*/
void MainWindow::main_slot_pickitInfo(QString asPicKitInfo)
{
    ui->pickitInfoTextArea->appendPlainText(asPicKitInfo);
}


