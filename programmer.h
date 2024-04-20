#ifndef PROGRAMMER_H
#define PROGRAMMER_H

#include <QMap>

class Programmer {
  public:
    Programmer();
    ~Programmer();
    void setProgrammer( const QString &programmer ) { activeProgrammer = programmer; };
    const QStringList &getCmd( const QString &action ) { return commands[ activeProgrammer ][ action ]; };
    bool isSupported( const QString &thisProgrammer ) { return commands.contains( thisProgrammer ); };
    bool supports( const QString &thisAction ) { return commands[ activeProgrammer ].contains( thisAction ); };

  private:
    QString activeProgrammer;
    QMap< QString, QMap< QString, QStringList > > commands;
};

#endif
