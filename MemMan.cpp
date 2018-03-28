#include <iostream>
#include "MemMan.h"
#include "mynew.h"
#include "MemCheck.h"


using namespace std;


MemBlock:: MemBlock()
{
  process = 0;
  startAddress = 0;
  size = 0;
}

MemBlock::MemBlock(int proc, int size) : process(proc), size(size) 
{
}

MemMan::MemMan(int ram, int proc, int op, MemCheck &memCheck) : totalRam(ram), totalProc(proc), totalOp(op) 
{
 
}// MemMan()


MemMan::~MemMan()
{
  
} // ~MemMan()



bool MemMan::access(int proc, int address, int opNum, MemCheck &memCheck, 
  char print)
{
  ListNode<MemBlock> *ptr = list.header->next;

  if(print != '0')
        cout << "Opnum: " << opNum << " access: proc: " << proc << " address: " << address << endl;

  while(ptr)
  {
    //cout << " in the access while loop" << endl;
    if(ptr->element.process == proc && ptr->element.startAddress <= address
       && address < (ptr->element.startAddress + ptr->element.size))
    {
      //if(print != '0')
      //  cout << "Opnum: " << opNum << " access: proc: " << proc << " address: " << address << endl;
      return true;
  // memCheck.printCurrentAllocations(proc);
 // memCheck.printOwner(address, address);
    }
    ptr = ptr->next;
  }

  endProc(proc, opNum, memCheck, print);
  return false;  
   
  // If seg fault, then free all memory assigned to the process, and return false.
  // If legitimate access, then return true;
}  // access()

int MemMan::alloc(int proc, int opNum, int size, MemCheck &memCheck, char print)
{
  int address = 0;  // to avoid warnings 
  
  // create new MemBlock base on input
  //MemBlock* data = new MemBlock(proc, size);
  MemBlock data = MemBlock(proc, size);
  //find where data should be inserted
  ListItr<MemBlock> itr = list.findPrevious(data);

  //if(!(itr.current->next)) // if end of list (no free space in list)
  //if(itr.advance().isPastEnd())
  //{
    // find start address
    data.startAddress = itr.current->element.startAddress + itr.current->element.size;
    //cout << "data->startAddress: " << data->startAddress << endl;
    // insert in the beginning
    list.insert(data, itr);//list.zeroth());
    address = data.startAddress;
   // cout << "data's start address: " << address << endl;

  /*} else { // find a lazy deleted block
    data.startAddress = itr.current->element.startAddress + itr.current->element.size;
    list.insert(data, itr);
    address = data.startAddress;
    //cout << "aclloc to deleted space. address: " << data->startAddress << endl;
    //list.insert(*data, itr);
  }
*/
  if(print != '0')
    cout << "Opnum: " << opNum << " alloc: proc: " << proc << " address: " 
      << address << " size: " << size << endl;
   //memCheck.printOwner(address, endAddress);
   //memCheck.printCurrentAllocations(proc);
  // allocates a block of the specified size, and returns its address.
  return address;
} // alloc()


void MemMan::deAlloc(int proc, int opNum, int startAddress, MemCheck &memCheck,
  char print)
{
  ListNode<MemBlock> *prev = list.header;
  ListNode<MemBlock> *ptr = list.header->next;

  while(ptr)
  {
    if(ptr->element.process == proc && ptr->element.startAddress == startAddress)
    {
      prev->next = ptr->next;
      //memCheck.deAlloc(proc, startAddress, opNum); // ptr->element.startAddress, ptr->element.operationNum);
      //delete &(ptr->element);
      delete ptr;
      //delete &(ptr->element);
      break;

    }
    prev = ptr;
    ptr = ptr->next;
  }

  if (print != '0')
    cout << "Opnum: " << opNum << " daAlloc: proc: " << proc << " startAddress: " << startAddress << endl;
  //  memCheck.printCurrentAllocations(proc);
  // memCheck.printOwner(startAddress, endAddress);
 
} // deAlloc()


void MemMan::endProc(int proc, int opNum, MemCheck &memCheck, char print)
{
  ListNode<MemBlock> *prev = list.header;
  ListNode<MemBlock> *ptr = list.header->next;

  while(ptr)
  {
    if(ptr->element.process == proc)
    {
      prev->next = ptr->next;
      memCheck.deAlloc(proc, ptr->element.startAddress, opNum); //ptr->element.operationNum);
      delete &(ptr->element);
      ptr = prev->next;
      continue;
    }
    prev = ptr;
    ptr = ptr->next;
  }

  if(print != '0')
    cout << "Opnum: " << opNum << " endProc: proc: " << proc << endl;
  
    //memCheck.printCurrentAllocations(proc);
  // free all memory assigned to proc.
} // endProc()


