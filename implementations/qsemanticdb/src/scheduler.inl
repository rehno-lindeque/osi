#ifdef  __QSEMANTICDB_SCHEDULER_H__
#ifndef __QSEMANTICDB_SCHEDULER_INL__
#define __QSEMANTICDB_SCHEDULER_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SCHEDULER.INL
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{
  Scheduler::Scheduler() : schedule(), evalSymbol(OSIX::SEMANTICID_INVALID)
  {
    activeQueue.push_back(schedule.Begin());
  }

  void Scheduler::Push(SemanticId symbol)
  {
    // todo: check this
    activeQueue.back()->PushBack(symbol);
    if(evalSymbol == OSIX::SEMANTICID_INVALID)
      evalSymbol = symbol;
  }

  void Scheduler::PushBranch(SemanticId symbol)
  {
    activeQueue.push_back(schedule.InsertBranch(activeQueue.back()));
    Push(symbol);
  }

  void Scheduler::CommitBranch()
  {
    for(auto i = activeQueue.rbegin(); i != activeQueue.rend(); ++i)
    {
      if((**i).QueryDepth() == 0)
        break;
      (**i).Commit();
    }
  }

  void Scheduler::RollbackBranch()
  {
    activeQueue.back()->Clear();
  }

  SemanticId Scheduler::Front()
  {
    //return schedule.Front();
    return evalSymbol;
  }

  SemanticId Scheduler::Pop()
  {
    // Get the next symbol in the root queue
    if(!schedule.Root().Empty())
    {
      SemanticId nextSymbol = schedule.Root().Front();
      schedule.Root().PopFront();
      return nextSymbol;
    }

    // If the symbol invalid, then the root queue is empty, so continue with the next available child branch
    // However, if the number of branches is 0, then the tree is empty, so the invalid symbol should be returned.
    if(schedule.Root().Branches() == 0)
      return OSIX::SEMANTICID_INVALID;

    // Continue with the next available branch
    auto iTree = schedule.Begin();
    ++iTree;

    // Condition: Since the number of branches > 0, the iterator should not be at the end of the list
    OSI_ASSERT(iTree != schedule.End());

    // Find a queue that can be committed
    while(iTree->QueryDepth() != 0 && iTree != schedule.End())
      ++iTree;

    if(iTree == schedule.End())
      return OSIX::SEMANTICID_INVALID;

    nextSymbol = iTree->Front();
    iTree->PopFront();
    if(iTree->Empty())
      schedule.RemoveRootBranch(iTree);

    // Condition: No queue that is not the root should ever be empty, hence the first symbol should probably not be invalid either
    OSI_ASSERT(nextSymbol != OSIX::SEMANTICID_INVALID);
    return nextSymbol;
  }
}

#endif
#endif
