#ifndef __QSEMANTICDB_SCHEDULER_H__
#define __QSEMANTICDB_SCHEDULER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SCHEDULER.H
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A scheduler maintaining the current state of the evaluator with the schedule.
*/
namespace QSemanticDB
{
  class Scheduler
  {
  public:
    //// Construction / Destruction
    Scheduler(Schedule& schedule);
    Scheduler() = delete;
    Scheduler(const Scheduler&) = delete;
    Scheduler & operator=(const Scheduler&) = delete;

    //// Query Operations
    void Push(SemanticId symbol);
    void PushInnerBranch(SemanticId symbol);
    void PushOuterBranch(SemanticId symbol);

    // Commit a branch for external evaluation
    void Commit();

    // Roll back an internally evaluated query
    void Rollback();

    //todo: SemanticId Back();

    // Eval Operations
    //SemanticId Front(); // This operation should not be supported (see Pop)

    // Get the current symbol
    SemanticId Get();

    //void Pop(); // This operation should not be supported in the scheduler, the scheduler is a "writer" object.

    void Reset() { activeQueue.clear(); activeQueue.push_back(schedule.Begin()); }
    bool Done() { return activeQueue.empty(); }

    //// Accessors
    //Schedule::TreeIterator FirstBranch() { return schedule.Begin(); }
    //Schedule::TreeConstIterator FirstBranch() const { return schedule.Begin(); }

  protected:
    Schedule& schedule;                               // The schedule tree structure
    SemanticId currentSymbol;
    std::vector<Schedule::TreeIterator> activeQueue;  // The stack of active queues (query strings)
    //SemanticId frontSymbol;                           // The current external eval symbol. This value is always equal to schedule.Front() but is cached here for efficiency.
    //SemanticId currentSymbol;                            // The current internal eval symbol (used with queries that require internal evaluation)
  };
}

/*                                   INCLUDES                               */
#include "scheduler.inl"

#endif
