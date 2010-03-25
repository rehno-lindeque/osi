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
    // Construction / Destruction
    Scheduler();
    Scheduler(const Scheduler&) = delete;
    Scheduler & operator=(const Scheduler&) = delete;

    // Operations
    void Push(SemanticId symbol);
    void PushBranch(SemanticId symbol);
    void CommitBranch();
    void RollbackBranch();
    SemanticId Front();
    SemanticId Pop();

    // Accessors

  protected:
    Schedule schedule;                                // The schedule tree structure
    std::vector<Schedule::TreeIterator> activeQueue;  // The stack of active queues (query strings)
    SemanticId evalSymbol;                            // The current eval symbol
  };
}

#endif
