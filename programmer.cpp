#include "programmer.h"

Programmer::Programmer() {
    // The mainwindow supports the target commands "Program", "Read", "Verify", "Detect", "Erase", blank "Check"
    // The PICkit2 window provides support for PICkit2 related commands "Info" and set "NewID"
    // Other progammer may provide only a subset of the target commands, the unsupported buttons will be hidden

    // PICkit2 target cmds
    commands[ "PICkit2" ][ "Program" ] = QStringList( { "pk2cmd", "-p", "-j", "-m", "-f" } );
    commands[ "PICkit2" ][ "Read" ] = QStringList( { "pk2cmd", "-p", "-j", "-gf" } );
    commands[ "PICkit2" ][ "Verify" ] = QStringList( { "pk2cmd", "-p", "-j", "-y", "-f" } );
    commands[ "PICkit2" ][ "DetectPIC" ] = QStringList( { "pk2cmd", "-p", "-i" } );
    commands[ "PICkit2" ][ "Erase" ] = QStringList( { "pk2cmd", "-p", "-e" } );
    commands[ "PICkit2" ][ "BlankCheck" ] = QStringList( { "pk2cmd", "-p", "-c" } );
    // PICkit2 programmer cmds
    commands[ "PICkit2" ][ "pk2Info" ] = QStringList( { "pk2cmd", "-s#" } );
    commands[ "PICkit2" ][ "pk2NewID" ] = QStringList( { "pk2cmd", "-n" } );

    // ArdPicProg target cmds
    commands[ "ArdPicProg" ][ "Program" ] = QStringList( { "ardpicprog", "--erase", "--burn", "-i" } );
    commands[ "ArdPicProg" ][ "Read" ] = QStringList( { "ardpicprog", "-o" } );
    commands[ "ArdPicProg" ][ "DetectPIC" ] = QStringList( { "ardpicprog", "--detect" } );
    commands[ "ArdPicProg" ][ "Erase" ] = QStringList( { "ardpicprog", "--erase" } );
}
