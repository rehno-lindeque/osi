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
  Scheduler::Scheduler(Schedule& schedule) : schedule(schedule), /*frontSymbol(OSIX::SEMANTICID_INVALID),*/ currentSymbol(OSIX::SEMANTICID_INVALID), queryDepth(0)
  {
    OSI_ASSERT(schedule.Begin() != schedule.End());
    activeQueue.push_back(schedule.Begin());
  }

  void Scheduler::Push(SemanticId symbol)
  {
    // Pre-condition: There should be at least one queue in the stack at the beginning
    OSI_ASSERT(!activeQueue.empty());

    // todo: check this
    activeQueue.back()->PushBack(symbol);
    //currentSymbol = symbol;
  }

  void Scheduler::PushInnerBranch(SemanticId symbol)
  {
    //activeQueue.push_back(schedule.InsertBranch(activeQueue.back())); (Note: pushing a branch does not mean that we will evaluate it next! Indeed we are simply adding it to the schedule of branches yet to be evaluated)
    auto iBranch = schedule.InsertInnerBranch(activeQueue.back());
    iBranch->PushBack(symbol);
  }

  void Scheduler::PushOuterBranch(SemanticId symbol)
  {
    //activeQueue.push_back(schedule.InsertBranch(activeQueue.back())); (Note: pushing a branch does not mean that we will evaluate it next! Indeed we are simply adding it to the schedule of branches yet to be evaluated)
    auto iBranch = schedule.InsertOuterBranch(activeQueue.back());
    iBranch->PushBack(symbol);
  }

  void Scheduler::Commit()
  {
    // Mark this branch and all parent branches as committed...
    for(auto i = activeQueue.rbegin(); i != activeQueue.rend(); ++i)
    {
      if((**i).QueryDepth() == 0)
        break;
      (**i).Commit();
    }

    // Continue to the next branch in the schedule tree
    while(activeQueue.size() > 0)
    {
      if(activeQueue.back()->Sibling() != schedule.End())
      {
        // Go to the next sibling
        activeQueue.back() = activeQueue.back()->Sibling();
        break;
      }

      // Go to the parent's next sibling (in the next iteration of this loop)
      activeQueue.pop_back();
    }
  }

  void Scheduler::Rollback()
  {
    // Invariant Condition: This always holds, but is rather relevant for the Rollback function
    OSI_ASSERT(activeQueue.back()->QueryDepth() == queryDepth);

    activeQueue.back()->Clear();
    if (schedule.RootBranches() > 1)
      schedule.RemoveLeafBranch(activeQueue.back());
    activeQueue.pop_back();

    // If there are no more queues left, then return
    if(activeQueue.empty())
      return;

    if(activeQueue.back()->QueryDepth() == queryDepth
          && activeQueue.back()->InnerBranches() == 0
          && activeQueue.back()->OuterBranches() == 0)
    {
      Rollback();
      return;
    }

    // Go to the next available child branch
    //todo: perhaps this should be: while(activeQueue.back()->OuterBranches() > 0)
    //      we should probably find a way of making sure the scheduler is ALWAYS at a leaf node (except when we are busy pushing new branches)
    if(activeQueue.back()->OuterBranches() > 0)
    {
      GotoFirstBranch();
      OSI_ASSERT(activeQueue.back()->OuterBranches() == 0);
    }
  }

  SemanticId Scheduler::Get()
  {
    OSI_ASSERT(!activeQueue.back()->Empty());
    return activeQueue.back()->Back();
  }

  /*SemanticId Scheduler::Front()
  {
    //return schedule.Front();
    return evalSymbol;
  }*/

  /*SemanticId Scheduler::Pop()
  {
    SemanticId prevEvalSymbol = evalSymbol;

    // Get the next symbol in the schedule
    OSI_ASSERT(prevEvalSymbol == schedule.Front());
    schedule.PopFront();
    evalSymbol = schedule.Front();
    return prevEvalSymbol;
  }*/
}

#endif
#endif
