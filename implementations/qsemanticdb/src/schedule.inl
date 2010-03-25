#ifdef __QSEMANTICDB_SCHEDULE_H__
#ifndef __QSEMANTICDB_SCHEDULE_INL__
#define __QSEMANTICDB_SCHEDULE_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SCHEDULE.INL
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{
  ScheduleQueue::ScheduleQueue(SymbolQueue* deque, uint queryDepth) : queue(queue), numberOfChildren(0), frontIndex(0), queryDepth(queryDepth)
  {}

  void ScheduleQueue::PushBack(SemanticId symbol)
  {
    queue->push_back(symbol);
  }

  void ScheduleQueue::PopFront()
  {
    OSI_ASSERT(!Empty());
    ++frontIndex;
    if(Empty())
      frontIndex = 0;
  }

  void ScheduleQueue::Clear()
  {
    queue->clear();
    frontIndex = 0;
  }

  void ScheduleQueue::Commit()
  {
    OSI_ASSERT(queryDepth > 0);
    --queryDepth;
  }

  SemanticId ScheduleQueue::Front() const
  {
    // Pre-condition: queue must not be empty
    OSI_ASSERT(!Empty());
    return (*queue)[frontIndex];
  }

  bool ScheduleQueue::Empty() const
  {
    return frontIndex == queue->size();
  }

  int ScheduleQueue::Branches() const
  {
    return numberOfChildren;
  }

  int ScheduleQueue::QueryDepth() const
  {
    return queryDepth;
  }


  Schedule::Schedule() : root(AllocQueue())
  {
    tree.push_back(ScheduleQueue(root, true));
  }

  Schedule::~Schedule()
  {
     Clear();
  }

  void Schedule::Clear()
  {
    while(!pool.empty())
    {
      delete pool.top();
      pool.pop();
    }
  }

  Schedule::TreeIterator Schedule::InsertBranch(Schedule::TreeIterator i)
  {
    ++i->numberOfChildren;
    tree.insert(i, ScheduleQueue(AllocQueue(), i->queryDepth));
  }

  void Schedule::RemoveRootBranch(Schedule::TreeIterator iChild)
  {
    // Note: this function is only intended for nodes branching directly from the root node
    --Begin()->numberOfChildren;
    OSI_ASSERT(Begin()->numberOfChildren >= 0);
    DeallocQueue(iChild->queue);
    tree.erase(iChild);
  }

  SemanticId Schedule::Front() const
  {
    if(!Root().Empty())
      return Root().Front();
    else
      return OSIX::SEMANTICID_INVALID;
  }

  Schedule::SymbolQueue* Schedule::AllocQueue()
  {
    // First try to find a deque in the pool of previously allocated deques
    if(!pool.empty())
    {
      SymbolQueue *result = pool.top();
      pool.pop();
      return result;
    }

    // Allocate a new queue if the pool is empty
    return new SymbolQueue;
  }

  void Schedule::DeallocQueue(SymbolQueue *queue)
  {
    pool.push(queue);
  }
}

#endif
#endif
