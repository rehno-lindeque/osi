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

    void Print(const char* title)
    {
      // Pre-conditions
      if(count > 99)
        return;

      // Initalize variables
      subgraphCounter = 0;

      // Open file stream
      std::ostringstream fileName;
      fileName << "debugscheduler" << std::setfill('0') << std::setw(2) << count << ".dot";
      std::ofstream fileStream(fileName.str());

      // Write graph data
      fileStream << "digraph G {" << std::endl;

      auto iEvalStack = scheduler.activeQueue.begin();
      auto iTree = scheduler.schedule.Begin();
      printRed = false;
      for(int rootBranchIndex = 0; rootBranchIndex < scheduler.schedule.RootBranches(); ++rootBranchIndex)
      {
        // Subgraph
        fileStream << " subgraph clusterRootSG" << subgraphCounter << " {" << std::endl;
        ++subgraphCounter;

        // Queue
        if(!printRed && iTree == *iEvalStack)
        {
          fileStream << "  node [color=red fontcolor=red];" << std::endl;
          printRed = true;
          ++iEvalStack;
        }
        else if(printRed && iTree != *iEvalStack)
        {
          fileStream << "  node [color=black fontcolor=black];" << std::endl;
          printRed = false;
        }
        PrintQueue(fileStream, iTree);
        fileStream << " }" << std::endl; // close subgraph

        // Branches
        PrintBranches(iEvalStack, fileStream, iTree);
        iTree = iTree->Sibling();
      }

      fileStream << " label = \"" << title << "\";" << std::endl << "}" << std::endl;
      ++count;
    }

    void Print(const char* title, Scheduler::Visitor &visitor)
    {
      SchedulerDebugVisualizer::visitor = &visitor;
      Print(title);
      SchedulerDebugVisualizer::visitor = 0;
    }

  protected:
    Scheduler& scheduler;
    bool printRed;
    int count;
    int subgraphCounter;
    Scheduler::Visitor *visitor;

    void PrintQueue(std::ofstream &fileStream, const Schedule::TreeIterator &iTree)
    {
      auto tree = *iTree;

      // Draw an empty node if the queue is empty
      if(tree.Empty())
      {
        fileStream << "  empty [shape=box];" << std::endl;
        return;
      }

      // Print the visited node if necessary
      PrintVisitedNode(fileStream, iTree/*, cSymbol*/);

      // Draw the string of symbols as a chained list of nodes
      fileStream << "  " << tree.Front();
      //PrintNodeShape(fileStream, iTree, tree.FrontIndex());
      for(uint cSymbol = tree.FrontIndex()+1; cSymbol < tree.Size(); ++cSymbol)
      {
        fileStream << " -> " << tree[cSymbol];
        //PrintNodeShape(fileStream, iTree, cSymbol);
      }
      fileStream << ';' << std::endl;
    }

    void PrintVisitedNode(std::ofstream &fileStream, const Schedule::TreeIterator &iTree/*, uint cSymbol*/)
    {
      if (visitor
          && visitor->GetTree() == iTree)
          //&& visitor->symbolIndex == cSymbol)
      {
        fileStream << (*iTree)[visitor->symbolIndex] << " [shape=hexagon]" << std::endl;
      }
    }

    void PrintBranches(Scheduler::QueueStackIterator iEvalStack, std::ofstream &fileStream, const Schedule::TreeIterator &iTree)
    {
      auto tree = *iTree;
      Schedule::TreeIterator iBranch = iTree;
      ++iBranch;
      /*for(uint cBranch = 0; cBranch < tree.InnerBranches(); ++cBranch)
      {
        auto branch = *iBranch;
        PrintQueue(fileStream, iTree);
        PrintBranches(fileStream, iTree);
        iBranch = iBranch->Sibling();
      }*/

      for(int cBranch = 0; cBranch < tree.OuterBranches(); ++cBranch)
      {
        if(subgraphCounter > 100)
          return;

        // Subgraph
        fileStream << " subgraph clusterSG" << subgraphCounter << " {" << std::endl;
        ++subgraphCounter;

        // Queue
        auto branch = *iBranch;
        if(!printRed && iTree == *iEvalStack)
        {
          fileStream << "  node [color=red fontcolor=red];" << std::endl;
          printRed = true;
          ++iEvalStack;
        }
        else if(printRed && iTree != *iEvalStack)
        {
          fileStream << "  node [color=black fontcolor=black];" << std::endl;
          printRed = false;
        }
        PrintQueue(fileStream, iBranch);
        fileStream << "  }" << std::endl; // close subgraph

        // Edge from the parent graph to this graph
        if(!tree.Empty() && !branch.Empty())
          fileStream << "  " << tree.Back() << " -> " << branch.Front() << ';' << std::endl;

        // Branches
        PrintBranches(iEvalStack, fileStream, iBranch);
        iBranch = iBranch->Sibling();
      }
    }
  };
}

/*                                   INCLUDES                               */
// Inline implementation
#include "scheduler.inl"

#endif
