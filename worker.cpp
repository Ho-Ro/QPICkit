#include "worker.h"
#include <QDebug>
#include <QProcess>

Worker::Worker( QObject *parent, Programmer *programmer ) : QObject( parent ), programmer( programmer ) { giInfoFlag = 0; }

/**
  Execute the pk2cmd command with the arguments arriving in an array.
  @param aobArguments - QStringList, array of arguments
*/
void Worker::worker_slot_executeCommand( QStringList aobArguments ) {
    gobProcess = new QProcess( this );
    QString lsProgram = aobArguments.takeFirst();
    QString lsCompleteCommand = lsProgram;
    QString lsExitStatus;
    bool lbReturnValue;

    for ( int liIndex = 0; liIndex < aobArguments.size(); liIndex++ ) {
        lsCompleteCommand += " " + aobArguments.at( liIndex );
    }
    if ( programmer->verbose )
        qDebug() << lsCompleteCommand;
    emit worker_signal_prepareCommandExecution();
    gobProcess->setProcessChannelMode( QProcess::MergedChannels );
    connect( gobProcess, SIGNAL( readyReadStandardOutput() ), this, SLOT( worker_slot_internalProcessOutputCapture() ) );
    gobProcess->start( lsProgram, aobArguments );
    if ( !gobProcess->waitForFinished() ) {
        lsExitStatus = "ERROR: " + this->gobProcess->errorString();
        lbReturnValue = false;
    } else {
        lsExitStatus = "OK";
        lbReturnValue = true;
    }
    giInfoFlag = 0;

    emit worker_signal_taskCompleted( lbReturnValue, lsExitStatus );
}

/**
  Execute the command to retrieve the information from the connected PICkit2.
*/
void Worker::worker_slot_pickitInfo() {
    giInfoFlag = 1;
    QStringList lobArguments;
    lobArguments = programmer->getCmd( "pk2Info" );
    if ( !lobArguments.isEmpty() )
        this->worker_slot_executeCommand( lobArguments );
}

/**
  Execute the command to set the new ID for the connected PICkit2.
*/
void Worker::worker_slot_pickitNewID( QString newID ) {
    giInfoFlag = 1;
    QStringList lobArguments;
    lobArguments = programmer->getCmd( "pk2NewID" );
    if ( lobArguments.isEmpty() )
        return;
    lobArguments.append( newID );
    this->worker_slot_executeCommand( lobArguments );
}

/**
  Sends the output of the executed command to the main thread.
*/
void Worker::worker_slot_internalProcessOutputCapture() {
    if ( giInfoFlag == 0 )
        emit worker_signal_processOutput( gobProcess->readAllStandardOutput() );
    else {
        emit worker_signal_pickitInfo( gobProcess->readAllStandardOutput() );
    }
}
