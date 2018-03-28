#ifndef MemManH
#define MemManH
#include "mynew.h"
#include "MemCheck.h"
#include "LinkedList.h"

class MemBlock
{
  public:
  int process, startAddress, size;

 // public:
  MemBlock();
  MemBlock(int proc, int size);
  friend class MemMan;
};

class MemMan
{
  int totalRam, totalProc, totalOp;
  List<MemBlock> list;

  public:
  MemMan(int ram, int proc, int op, MemCheck &memCheck);
  ~MemMan();
  bool access(int proc, int address, int opNum, MemCheck &memCheck, char print);
  void endProc(int proc, int opNum, MemCheck &memCheck, char print);
  int alloc(int proc, int opNum, int size, MemCheck &memCheck, char print);
  void deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck, char print );
}; // class MemMan

#endif
