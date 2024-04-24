#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <QMap>
#include <QString>
#include <QStringList>

class Programmer {
  public:
    Programmer();
    ~Programmer() {}
    void init();
    void setProgrammer( const QString &programmer ) { activeProgrammer = programmer; }
    const QStringList &getCmd( const QString &action ) { return Programmer::commands[ activeProgrammer ][ action ]; }
    bool isSupported( const QString &thisProgrammer ) { return Programmer::commands.contains( thisProgrammer ); }
    bool supportsCmd( const QString &thisAction ) { return Programmer::commands[ activeProgrammer ].contains( thisAction ); }
    bool verbose = false;

  private:
    QMap< QString, QMap< QString, QStringList > > commands;
    QString activeProgrammer;
};

#endif // PROGRAMMER_H
