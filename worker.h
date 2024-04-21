#ifndef WORKER_H
#define WORKER_H

#include "programmer.h"
#include <QObject>
#include <QProcess>

class Worker : public QObject {
    Q_OBJECT
  public:
    explicit Worker( QObject *parent = nullptr, Programmer *programmer = nullptr );

  signals:
    void worker_signal_processOutput( QString output );
    void worker_signal_pickitInfo( QString info );
    void worker_signal_prepareCommandExecution();
    void worker_signal_taskCompleted( bool abExitStatus, QString asExitString );

  public slots:
    void worker_slot_executeCommand( QStringList aobArguments );
    void worker_slot_pickitInfo();
    void worker_slot_pickitNewID( QString newID );
    void worker_slot_internalProcessOutputCapture();

  private:
    int giInfoFlag;
    QProcess *gobProcess;
    Programmer *programmer;
};

#endif // WORKER_H
