#ifndef __QSEMANTICDB_SCHEDULE_H__
#define __QSEMANTICDB_SCHEDULE_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SCHEDULE.H
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A schedule for evaluating queries and definitions. Implemented as a
      tree-like queue with commit and rollback actions.
*/
namespace QSemanticDB
{
  class ScheduleQueue
  {
    friend class Schedule;
  public:
    // Types
    typedef std::deque<SemanticId> SymbolDeque;

    // Construction / Destruction
    ScheduleQueue(SymbolDeque* deque) : deque(deque), numberOfChildren(0) {}
    ScheduleQueue() = delete;
    ScheduleQueue(const ScheduleQueue&) = default;
    ScheduleQueue& operator=(const ScheduleQueue&) = default;

    // Operations
    void PushBack(SemanticId symbol)
    {
      deque->push_back(symbol);
    }

    void PopFront()
    {
      deque->pop_front();
    }

    void Clear()
    {
      deque->clear();
    }

    // Accessors
    SemanticId Front() const
    {
      return deque->front();
    }

    bool Empty() const
    {
      return deque->empty();
    }

  protected:
    SymbolDeque* deque;
    uint numberOfChildren;
  };

  class Schedule
  {
  public:
    // Types
    typedef std::deque<SemanticId> SymbolDeque;

    // Construction / Destruction
    Schedule() : root(AllocDeque()) { branches.push_back(ScheduleQueue(root)); }
    Schedule(const Schedule&) = delete;
    Schedule & operator=(const Schedule&) = delete;

    ~Schedule() { Clear(); }

    // Operations
    void Clear()
    {
      while(!dequePool.empty())
      {
        delete dequePool.top();
        dequePool.pop();
      }
    }

    // Accessors
    SemanticId Front() const
    {
      if(!Root().Empty())
        return Root().Front();
      else
        return OSIX::SEMANTICID_INVALID;
    }

    //SymbolDeque& Root() { return *root; }
    //const SymbolDeque& Root() const { return *root; }

    ScheduleQueue& Root() { return branches.front(); }
    const ScheduleQueue& Root() const { return branches.front(); }

  protected:

    // The root of the hierarchy
    SymbolDeque *root;
    std::list<ScheduleQueue> branches;

    // Pool of deques
    /*struct DequePoolElement
    {
      inline DequePoolElement() : deque(new deque), isInUse(false) {}
      SymbolDeque *deque;
      bool isInUse;
    };*/
    typedef SymbolDeque* DequePoolElement;
    typedef std::priority_queue<DequePoolElement> DequePool;
    DequePool dequePool;

    // Memory allocation
    SymbolDeque *AllocDeque()
    {
      // First try to find a deque in the pool of previously allocated deques
      if(!dequePool.empty())
      {
        SymbolDeque *result = dequePool.top();
        dequePool.pop();
        return result;
      }

      // Allocate a new deque if the pool is empty
      return new SymbolDeque;
    }
  };
}

#endif
