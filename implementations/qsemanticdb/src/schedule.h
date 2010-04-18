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
    //typedef SymbolQueue::iterator SymbolIterator;
    //typedef SymbolQueue::const_iterator SymbolConstIterator;
    typedef std::list<ScheduleQueue> Tree;
    typedef Tree::iterator TreeIterator;
    typedef Tree::const_iterator TreeConstIterator;

    // Construction / Destruction
    ScheduleQueue(SymbolQueue* queue, uint queryDepth, const TreeIterator& iSibling);
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
    SemanticId Back() const;
    bool Empty() const;
    size_t Size() const;
    //int Branches() const;
    int InnerBranches() const;
    int OuterBranches() const;
    int QueryDepth() const;
    void QueryDepth(int depth);
    const TreeIterator& Sibling();
    TreeConstIterator Sibling() const;
    int FrontIndex() const;
    SemanticId operator [] (int index) const;
    /*SymbolIterator Begin();
    SymbolConstIterator Begin() const;
    SymbolIterator End();
    SymbolConstIterator End() const;*/

  protected:
    SymbolQueue* queue;
    int queryDepth;         // The number of queries this queue is involved in. Every time a query is completed, the count goes down by one. If the count is 0, the queue may be fully evaluated.
    int nInnerBranches;     // Number of "inner" (child) branches
    int nOuterBranches;     // Number of "outer" (parent) branches
    size_t frontIndex;      // Index of the first symbol (since this is implemented using a vector, the front elements are not deleted when they get popped from the front of the queue. Instead the index is moved forward).
    TreeIterator iSibling;  // Iterator to the next sibling in the tree
  };

  class Schedule
  {
    friend class Scheduler;
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
    //TreeIterator InsertBranch(TreeIterator iBranch);
    TreeIterator InsertInnerBranch(TreeIterator iBranch);
    TreeIterator InsertOuterBranch(TreeIterator iBranch);
    void RemoveFirstLeafBranch(const TreeIterator& iBranch);
    void PopFront();

    // Accessors
    SemanticId Front() const;

    //SymbolDeque& Root() { return *root; }
    //const SymbolDeque& Root() const { return *root; }

    //ScheduleQueue& Root() { return tree.front(); }
    //const ScheduleQueue& Root() const { return tree.front(); }

    int RootBranches() const;
    bool Empty() const;

    TreeConstIterator Begin() const { return tree.begin(); }
    TreeConstIterator End() const { return tree.end(); }
    TreeIterator Begin() { return tree.begin(); }
    TreeIterator End() { return tree.end(); }

  protected:

    // The root of the hierarchy
    //SymbolQueue *root;
    int nRootBranches;   // The number of root branches
    Tree tree;            // A tree of queues

    // Pool of deques
    struct lessPriority : public std::binary_function<SymbolQueue*, SymbolQueue*, bool>
    {
      bool operator()(const SymbolQueue* arg1, const SymbolQueue* arg2) const { return arg1->capacity() < arg2->capacity(); }
    };

    typedef SymbolQueue* PoolElement;
    typedef std::priority_queue<PoolElement, std::vector<PoolElement>, lessPriority> Pool;
    Pool pool;

    // Internal operations
    //void PopRoot();

    // Collapse the first branch in the root of the tree (replace it by its child branches)
    void CollapseFirstRootBranch();

    // Collapse any branch in the root of the tree (replace it by its child branches)
    // Note: We're avoiding deleting any root branch since it is expensive to update the previous sibling in the list since the list is only singly-linked
    //       However, if we start evaluating branches concurrently this functionality will become necessary
    //void CollapseRootBranch(TreeIterator iBranch);

    // Memory allocation
    SymbolQueue *AllocQueue();
    void DeallocQueue(SymbolQueue *queue);
  };
}

/*                                   INCLUDES                               */
// Inline implementation
#include "schedule.inl"

#endif
