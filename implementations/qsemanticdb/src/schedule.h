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
    typedef std::vector<SemanticId> SymbolQueue;

    // Construction / Destruction
    ScheduleQueue(SymbolQueue* deque, uint queryDepth);
    ScheduleQueue() = delete;
    ScheduleQueue(const ScheduleQueue&) = default;
    ScheduleQueue& operator=(const ScheduleQueue&) = default;

    // Operations
    void PushBack(SemanticId symbol);
    void PopFront();
    void Clear();
    void Commit();

    // Accessors
    SemanticId Front() const;
    bool Empty() const;
    int Branches() const;
    int QueryDepth() const;

  protected:
    SymbolQueue* queue;
    int queryDepth;         // The number of queries this queue is involved in. Every time a query is completed, the count goes down by one. If the count is 0, the queue may be fully evaluated.
    int numberOfChildren;   // Number of child branches
    int frontIndex;
  };

  class Schedule
  {
  public:
    // Types
    //typedef std::deque<SemanticId> SymbolDeque;
    typedef std::vector<SemanticId> SymbolQueue;
    typedef std::list<ScheduleQueue> Tree;
    typedef Tree::iterator TreeIterator;
    typedef Tree::const_iterator TreeConstIterator;

    // Construction / Destruction
    Schedule();
    Schedule(const Schedule&) = delete;
    Schedule & operator=(const Schedule&) = delete;

    ~Schedule();

    // Operations
    void Clear();

    TreeIterator InsertBranch(TreeIterator i);

    void RemoveRootBranch(TreeIterator iChild);

    // Accessors
    SemanticId Front() const;

    //SymbolDeque& Root() { return *root; }
    //const SymbolDeque& Root() const { return *root; }

    ScheduleQueue& Root() { return tree.front(); }
    const ScheduleQueue& Root() const { return tree.front(); }

    TreeConstIterator Begin() const { return tree.begin(); }
    TreeConstIterator End() const { return tree.end(); }
    TreeIterator Begin() { return tree.begin(); }
    TreeIterator End() { return tree.end(); }

  protected:

    // The root of the hierarchy
    SymbolQueue *root;
    Tree tree;

    // Pool of deques
    /*struct DequePoolElement
    {
      inline DequePoolElement() : deque(new deque), isInUse(false) {}
      SymbolDeque *deque;
      bool isInUse;
    };*/
    struct lessPriority : public std::binary_function<SymbolQueue*, SymbolQueue*, bool>
    {
      bool operator()(const SymbolQueue* arg1, const SymbolQueue* arg2) const { return arg1->capacity() < arg2->capacity(); }
    };

    typedef SymbolQueue* PoolElement;
    typedef std::priority_queue<PoolElement, std::vector<PoolElement>, lessPriority> Pool;
    Pool pool;

    // Memory allocation
    SymbolQueue *AllocQueue();
    void DeallocQueue(SymbolQueue *queue);
  };
}

#endif
