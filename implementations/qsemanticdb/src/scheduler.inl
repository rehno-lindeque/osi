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

  void Scheduler::BeginQuery()
  {
    ++queryDepth;
    activeQueue.back()->QueryDepth(queryDepth);
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
    GotoNextBranch();
  }

  void Scheduler::Rollback()
  {
    // Invariant Condition: This always holds, but is rather relevant for the Rollback function
    OSI_ASSERT(activeQueue.back()->QueryDepth() == queryDepth);

    // Remove the branch
    activeQueue.back()->Clear();
    if (activeQueue.size() > 1)
    {
      // TODO: this function does not work correctly
      // TODO: cannot use remove "First" branch because we're not necessarily at the first branch
      schedule.RemoveFirstLeafBranch(activeQueue.back());
      OSI_ASSERT(false);
    }
    else if (schedule.RootBranches() > 1)
    {
      // TODO: cannot use collapse "First" branch because we're not necessarily at the first branch
      //schedule.CollapseFirstRootBranch();
      OSI_ASSERT(false);
    }

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

  void Scheduler::GotoFirstBranch()
  {
    Schedule::TreeIterator iChild = activeQueue.back(); ++iChild;
    activeQueue.push_back(iChild);
  }

  void Scheduler::GotoNextBranch()
  {
    while(activeQueue.size() > 0)
    {
      if(activeQueue.back()->Sibling() != schedule.End())
      {
        // Go to the next sibling
        activeQueue.back() = activeQueue.back()->Sibling();
        //QSEMANTICDB_DEBUG_EVALOUTPUT_PRINT("Commit...Sibling" << std::endl)
        break;
      }

      // Go to the parent's next sibling (in the next iteration of this loop)
      activeQueue.pop_back();
      //QSEMANTICDB_DEBUG_EVALOUTPUT_PRINT("Commit...Pop" << std::endl)
    }
  }

  /*SemanticId Scheduler::Front()
  {
    //return schedule.Front();
    return evalSymbol;
  }*/

  SemanticId Scheduler::Get()
  {
    OSI_ASSERT(!activeQueue.back()->Empty());
    return activeQueue.back()->Back();
  }

  /*SemanticId Scheduler::Pop()
  {
    SemanticId prevEvalSymbol = evalSymbol;

    // Get the next symbol in the schedule
    OSI_ASSERT(prevEvalSymbol == schedule.Front());
    schedule.PopFront();
    evalSymbol = schedule.Front();
    return prevEvalSymbol;
  }*/


  void Scheduler::Reset()
  {
    activeQueue.clear();
    activeQueue.push_back(schedule.Begin());
  }

  bool Scheduler::Done() const
  {
    return activeQueue.empty() || activeQueue.back()->Empty();
  }

  int Scheduler::InnerBranches() const
  {
    return activeQueue.back()->InnerBranches();
  }

  int Scheduler::OuterBranches() const
  {
    return activeQueue.back()->OuterBranches();
  }

  int Scheduler::QueryDepth() const
  {
    return queryDepth;
  }

  Scheduler::Visitor Scheduler::GetVisitor()
  {
    // Pre-condition: Current branch must have at least 1 symbol
    OSI_ASSERT(activeQueue.size() > 1 && activeQueue.back()->Size() > 0);
    return Visitor(*this, activeQueue.size()-1, activeQueue.back()->Size()-1);
  }

}

#endif
#endif
