#ifndef __QSEMANTICDB_SCHEDULERDEBUGVISUALIZER_H__
#define __QSEMANTICDB_SCHEDULERDEBUGVISUALIZER_H__
//////////////////////////////////////////////////////////////////////////////
//
//    SCHEDULERDEBUGVISUALIZER.H
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////
/*                               DOCUMENTATION                              */
/*
    DESCRIPTION:
      A debugging visualizer for the scheduler
*/
namespace QSemanticDB
{
  class SchedulerDebugVisualizer
  {
  public:
    SchedulerDebugVisualizer(Scheduler& scheduler) : scheduler(scheduler), count(0) {}
    void Print()
    {
      std::ostringstream fileName;
      fileName << "debugscheduler" << std::setfill('0') << std::setw(2) << count << ".dot";

      std::ofstream fileStream(fileName.str());
      fileStream << "test";
    }

  protected:
    Scheduler& scheduler;
    int count;
  };
}

/*                                   INCLUDES                               */
// Inline implementation
#include "scheduler.inl"

#endif
