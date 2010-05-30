#ifdef  __QSEMANTICDB_SCHEDULERDEBUGVISUALIZER_H__
#ifndef __QSEMANTICDB_SCHEDULERDEBUGVISUALIZER_INL__
#define __QSEMANTICDB_SCHEDULERDEBUGVISUALIZER_INL__
//////////////////////////////////////////////////////////////////////////////
//
//    SCHEDULERDEBUGVISUALIZER.INL
//
//    Copyright © 2010, Rehno Lindeque. All rights reserved.
//
//////////////////////////////////////////////////////////////////////////////

namespace QSemanticDB
{
  void SchedulerDebugVisualizer::Print(const char* title)
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
    //fileStream << "  rankdir=TB;" << std::endl;
    fileStream << "  rankdir=LR;" << std::endl;

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

    // Print the query stack
    PrintQueryStack(fileStream);

    // Print the active queue
    PrintActiveQueue(fileStream);

    // TODO: Write additional info


    // Write graph title
    fileStream << " label = \"" << title << "\";" << std::endl << "}" << std::endl;

    // Increment the file count
    ++count;
  }

  void SchedulerDebugVisualizer::Print(const char* title, Scheduler::Visitor &visitor)
  {
    SchedulerDebugVisualizer::visitor = &visitor;
    Print(title);
    SchedulerDebugVisualizer::visitor = 0;
  }

  void SchedulerDebugVisualizer::PrintAtom(std::ofstream &fileStream, SemanticId atom)
  {
    auto i = db.epsilonStrings.find(atom);
    if(i != db.epsilonStrings.end())
    {
      fileStream << i->second;
    }
    else
    {
      fileStream << "\""
                 << (atom == SemanticDBImplementation::INTERNALID_HIDDEN? "Hidden" : "Anon")
                 << "(" << atom << ")\"";
    }
  }

  void SchedulerDebugVisualizer::InternPrintSymbol(std::ofstream &fileStream, SemanticId symbol)
  {
    Relation unqualifiedRelation(OSIX::SEMANTICID_INVALID, OSIX::SEMANTICID_INVALID);
    if(!db.GetUnqualifiedRelation(symbol, unqualifiedRelation))
    {
      PrintAtom(fileStream, symbol);
    }
    else if (unqualifiedRelation.domain == OSIX::SEMANTICID_INVALID || unqualifiedRelation.domain == OSIX::SEMANTICID_EPSILON)
    {
      PrintAtom(fileStream, symbol);
    }
    else
    {
      InternPrintSymbol(fileStream, unqualifiedRelation.domain);
      fileStream << " &#8594; ";
      //fileStream << " -$gt; ";
      InternPrintSymbol(fileStream, unqualifiedRelation.codomain);
    }
  }

  void SchedulerDebugVisualizer::PrintSymbol(std::ofstream &fileStream, SemanticId symbol)
  {
    fileStream << "\"";
    InternPrintSymbol(fileStream, symbol);
    fileStream << "\"";
  }

  void SchedulerDebugVisualizer::PrintQueue(std::ofstream &fileStream, const Schedule::TreeIterator &iTree)
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
    fileStream << "  ";
    PrintSymbol(fileStream, tree.Front());

    //PrintNodeShape(fileStream, iTree, tree.FrontIndex());
    for(uint cSymbol = tree.FrontIndex()+1; cSymbol < tree.Size(); ++cSymbol)
    {
      fileStream << " -> ";
      PrintSymbol(fileStream, tree[cSymbol]);
      //PrintNodeShape(fileStream, iTree, cSymbol);
    }
    fileStream << ';' << std::endl;
  }

  void SchedulerDebugVisualizer::PrintVisitedNode(std::ofstream &fileStream, const Schedule::TreeIterator &iTree/*, uint cSymbol*/)
  {
    if (visitor
        && visitor->GetTree() == iTree)
        //&& visitor->symbolIndex == cSymbol)
    {
      PrintSymbol(fileStream, (*iTree)[visitor->symbolIndex]);
      fileStream << " [shape=hexagon]" << std::endl;
    }
  }

  void SchedulerDebugVisualizer::PrintBranches(Scheduler::QueueStackIterator iEvalStack, std::ofstream &fileStream, const Schedule::TreeIterator &iTree)
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
      fileStream << " }" << std::endl; // close subgraph

      // Edge from the parent graph to this graph
      if(!tree.Empty() && !branch.Empty())
        fileStream << "  ";
        PrintSymbol(fileStream, tree.Back());
        fileStream << " -> ";
        //fileStream << "SG" << (subgraphCounter-1) << ':';
        PrintSymbol(fileStream, branch.Front());
        fileStream << " [lhead=SG" << (subgraphCounter-1) << "]";
        fileStream << ';' << std::endl;

      // Branches
      PrintBranches(iEvalStack, fileStream, iBranch);
      iBranch = iBranch->Sibling();
    }
  }

  void SchedulerDebugVisualizer::PrintQueryStack(std::ofstream &fileStream)
  {
    if(db.evalQueryStack.empty())
      return;

    fileStream  << " subgraph miscQueryStack {" << std::endl;
    fileStream  << " queryStack [shape=none, margin=0, label=<" << std::endl
                << "   <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">" << std::endl;
    for(auto i = db.evalQueryStack.rbegin(); i != db.evalQueryStack.rend(); ++i)
    {
      fileStream << "    <TR><TD>";
      InternPrintSymbol(fileStream, *i);
      fileStream << "</TD></TR>";
    }
    fileStream  << std::endl << "    <TR><TD BGCOLOR=\"lightgrey\">QUERY STACK</TD></TR>";
    fileStream  << "</TABLE>" << std::endl
                << " >];" << std::endl;
    fileStream  << " }" << std::endl;
  }

  void SchedulerDebugVisualizer::PrintActiveQueue(std::ofstream &fileStream)
  {
    if(db.scheduler.activeQueue.empty())
      return;

    fileStream  << " subgraph miscActiveQueue {" << std::endl;
    fileStream  << " activeQueue [shape=none, margin=0, label=<" << std::endl
                << "   <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">" << std::endl;
    for(auto iQueue = db.scheduler.activeQueue.rbegin(); iQueue != db.scheduler.activeQueue.rend(); ++iQueue)
    {
      fileStream << "    <TR>";
      auto scheduleQueue = **iQueue;
      for(int c = scheduleQueue.FrontIndex(); c < scheduleQueue.Size(); ++c)
      {
        fileStream << "<TD>";
        InternPrintSymbol(fileStream, scheduleQueue[c]);
        fileStream << "</TD>";
      }
      fileStream << "</TR>";
    }
    fileStream  << std::endl << "    <TR><TD BGCOLOR=\"lightgrey\">ACTIVE QUEUES</TD></TR>";
    fileStream  << "</TABLE>" << std::endl
                << " >];" << std::endl;
    fileStream  << " }" << std::endl;
  }
}

#endif
#endif
