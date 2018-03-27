/////////////////////////////////////////////////////////////////
// Graph.cpp - Demonstrate how to do Graph walks               //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////

#include "Graph.h"

//----< utilities >----------------------------------------------------------
void title(const std::string& t, char underline = '-')
{
  std::cout << "\n  " << t;
  std::cout << "\n " << std::string(t.size() + 2, underline);
}

void putLine(size_t lines = 1)
{
  for (size_t i = 0; i < lines; ++i)
    std::cout << "\n";
}

void putLine_Indent()
{
  std::cout << "\n  ";
}

//----< demonstration >------------------------------------------------------
#ifdef TEST_GRAPH
int main()
{
  title("Demonstrate Graph Traversal Strategies", '=');
  putLine();
  using Graph = Graph<std::string>;
  using Node = Node<std::string>;
  using Sptr_graph = Sptr_graph<std::string>;

  Graph graph("testing Graph");

  graph.addNode(Sptr_graph(new Node("DateTime.cpp")));
  graph.addNode(Sptr_graph(new Node("DateTime.h")));
  graph.addNode(Sptr_graph(new Node("DbCore.cpp")));
  graph.addNode(Sptr_graph(new Node("DbCore.h")));
  graph.addNode(Sptr_graph(new Node("Query.cpp")));
  graph.addNode(Sptr_graph(new Node("Query.h")));
  graph.addNode(Sptr_graph(new Node("TestExecutive.cpp")));
  graph.addNode(Sptr_graph(new Node("PayLoad.h")));
  graph.addNode(Sptr_graph(new Node("XmlDocument.h")));

  graph.addEdge("DbCore.h", "DbCore.h");
  graph.addEdge("DbCore.h", "DateTime.h");
  graph.addEdge("DbCore.h", "Query.h");
  graph.addEdge("Query.h", "XmlDocument.h");
  graph.addEdge("TestExecutive.cpp", "DbCore.h");
  graph.addEdge("TestExecutive.cpp", "Query.h");
  graph.addEdge("TestExecutive.cpp", "PayLoad.h");

  showGraph<std::string>(graph);

  auto proc = [](Sptr_graph pNode) { std::cout << " --> " << pNode->name(); };
  using P = decltype(proc);
  using V = std::string;

  getchar();
  putLine(3);
}
#endif