#include "programmer.h"


Programmer::Programmer() {
    commands[ "PICkit2" ][ "Program" ] = QStringList( { "pk2cmd", "-p", "-j", "-m", "-f" } );
    commands[ "PICkit2" ][ "Read" ] = QStringList( { "pk2cmd", "-p", "-j", "-gf" } );
    commands[ "PICkit2" ][ "Verify" ] = QStringList( { "pk2cmd", "-p", "-j", "-y", "-f" } );
    commands[ "PICkit2" ][ "Detect" ] = QStringList( { "pk2cmd", "-p", "-i" } );
    commands[ "PICkit2" ][ "Erase" ] = QStringList( { "pk2cmd", "-p", "-e" } );
    commands[ "PICkit2" ][ "Check" ] = QStringList( { "pk2cmd", "-p", "-c" } );

    commands[ "ArdPicProg" ][ "Program" ] = QStringList( { "ardpicprog", "--erase", "--burn", "-i" } );
    commands[ "ArdPicProg" ][ "Read" ] = QStringList( { "ardpicprog", "-o" } );
    commands[ "ArdPicProg" ][ "Detect" ] = QStringList( { "ardpicprog", "--detect" } );
    commands[ "ArdPicProg" ][ "Erase" ] = QStringList( { "ardpicprog", "--erase" } );
};


Programmer::~Programmer(){};
