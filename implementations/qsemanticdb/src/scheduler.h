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
#ifdef QSEMANTICDB_DEBUG_VISUALIZE
    friend class SchedulerDebugVisualizer;
#endif

    //// Types
    typedef std::vector<Schedule::TreeIterator> QueueStack;
    typedef QueueStack::iterator QueueStackIterator;
    typedef QueueStack::const_iterator QueueStackConstIterator;
    typedef Schedule::Tree Tree;
    typedef Schedule::TreeIterator TreeIterator;
    typedef Schedule::TreeConstIterator TreeConstIterator;

    //// Construction / Destruction
    Scheduler(Schedule& schedule);
    Scheduler() = delete;
    Scheduler(const Scheduler&) = delete;
    Scheduler & operator=(const Scheduler&) = delete;

    //// Query Operations
    void Push(SemanticId symbol);
    void PushInnerBranch(SemanticId symbol);
    void PushOuterBranch(SemanticId symbol);

    // Increment the query depth;
    void BeginQuery();

    // Commit a branch for external evaluation
    void Commit();

    // Roll back an internally evaluated query (recursively for parent queues)
    void Rollback();

    // Go to the first branch of the current queue
    // (Currently only outer branches supported)
    void GotoFirstBranch();

    // Go to the next branch
    void GotoNextBranch();

    //todo: SemanticId Back();

    // Eval Operations
    //SemanticId Front(); // This operation should not be supported (see Pop)

    // Get the current symbol
    SemanticId Get();

    //void Pop(); // This operation should not be supported in the scheduler, the scheduler is a "writer" object.

    void Reset();
    bool Done() const;
    int InnerBranches() const;
    int OuterBranches() const;
    int QueryDepth() const;

    //// Accessors
    // Schedule::TreeIterator FirstBranch() { return schedule.Begin(); }
    // Schedule::TreeConstIterator FirstBranch() const { return schedule.Begin(); }

    //// Visitor
    class Visitor
    {
    public:
      // Construction / Destruction
      Visitor(Scheduler& scheduler, size_t queueStackIndex, size_t symbolIndex) : scheduler(&scheduler), queueStackIndex(queueStackIndex), symbolIndex(symbolIndex) {}
      Visitor(const Visitor&) = default;
      Visitor() = delete;

      // Accessors
      TreeIterator GetTree() const { return scheduler->activeQueue[queueStackIndex]; }
      SemanticId Get() const { return (*GetTree())[symbolIndex]; }
      bool EndOfQueue() const { return GetTree()->Size() == symbolIndex+1; }
      bool EndOfBranch() const { return queueStackIndex == scheduler->activeQueue.size(); }

      Scheduler* scheduler;
      //TreeIterator tree;
      size_t queueStackIndex;
      //ScheduleQueue::SymbolIterator symbol;
      size_t symbolIndex;
    };

    Visitor GetVisitor();

  //protected:
    Schedule& schedule;                           // The schedule tree structure
    SemanticId currentSymbol;                     // The current symbol being evaluated
    QueueStack activeQueue;                       // The stack of active queues (query strings)
    int queryDepth;                               // The number of nested, but unresolved queries in the activeQueue.
    // SemanticId frontSymbol;                    // The current external eval symbol. This value is always equal to schedule.Front() but is cached here for efficiency.
    // SemanticId currentSymbol;                  // The current internal eval symbol (used with queries that require internal evaluation)
  };
}

/*                                   INCLUDES                               */
// Inline implementation
#include "scheduler.inl"

#endif
