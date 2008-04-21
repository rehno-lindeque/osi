#ifndef __BASEGE_MEMORYPOOL_H__
#define __BASEGE_MEMORYPOOL_H__
//////////////////////////////////////////////////////////////////////////////
//
//    MEMORYPOOL.H
//
//    Copyright © 2006, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      BaseGE Memory Pool
*/

/*                                 INCLUDES                                */
namespace BaseGE
{
/*                                 CONSTANTS                                */
#define BASEGE_MEMORY_BLOCK_SIZE (1024*1024)

/*                                  CLASSES                                 */
  class MemoryPool
  {
  public:
    std::list<void*> memoryList;
    std::multimap<int, void*> blocks;
    //vector<void*> open
    
    void* activeBlock;
    void* activePosition;
    int activeSize;

    inline MemoryPool()
    {
      void* memory = malloc(BASEGE_MEMORY_BLOCK_SIZE);
      memoryList.push_back(memory);
      blocks.insert(std::pair<int, void*>(BASEGE_MEMORY_BLOCK_SIZE, memory));
      activeBlock = memory;
      activeSize = BASEGE_MEMORY_BLOCK_SIZE;
    }

    inline ~MemoryPool()
    {
      for(std::list<void*>::iterator i = memoryList.begin(); i != memoryList.end(); ++i)
        free(*i);
    }
  };
/*                                  GLOBALS                                 */
  extern MemoryPool memoryPool;

/*                                PROTOTYPES                                */
  inline void* allocateStaticMemory(int size)
  {
    if(memoryPool.activeSize < size)
      throw 0; // error: out of memory
    
    memoryPool.activeSize -= size;
    void* memory = memoryPool.activeBlock;
    memoryPool.activeBlock = (OSuint8*)memoryPool.activeBlock + size;

    return memory;
  }

  //todo:
  /*inline int createDynamicMemory()
  {
  }
  
  inline void* resizeDynamicMemory(int memoryId, int size)
  {

  }

  //todo: openDynamicMemory(int memoryId)

  inline void* closeDynamicMemory(int memoryId)
  {

  }*/
}

#endif
