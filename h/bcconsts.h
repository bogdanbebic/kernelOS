#ifndef _BCC_CONSTS_H_
#define _BCC_CONSTS_H_

/* BATCH FILES FOR COMPILATION:

// cmp.bat:
@echo off
cd c:\bc31\bin
bcc +C:\bc31\BIN\vscode\batch\cmpsw.txt %1 APPLICAT.LIB
@echo on
// ! end cmp.bat

// cmpsw.txt:
-mh -y -v -Ic:\bc31\include;C:\bc31\BIN\vscode\lib;C:\bc31\BIN\vscode\h -Lc:\bc31\lib;C:\bc31\BIN\vscode\lib -nC:\bc31\BIN\vscode\output -evs_test.exe
// ! end cmpsw.txt

// dbg.bat:
@echo off
cd c:\bc31\bin
td.exe -ctdconfig.td -sdC:\bc31\BIN\vscode\src C:\bc31\BIN\vscode\output\vs_test.exe
@echo on
// ! end dbg.bat

*/


#ifndef BCC_BLOCK_IGNORE

#define nullptr 0

typedef int bool;

extern bool true;
extern bool false;

typedef unsigned int Word;

#endif

#endif
