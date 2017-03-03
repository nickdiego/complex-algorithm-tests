#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cassert>
#include <cstdio>

#if 1
#define PROJ_AMOUNT 5
#define MAX_WORKLOAD 100
#else
#define PROJ_AMOUNT 6
#define 120
#endif

#if ENABLE_DEBUG
#define TRACE(...) fprintf(stderr, __VA_ARGS__)
#else
#define TRACE(...)
#endif

using std::vector;
using std::cout;
using std::endl;

struct Project {
  int workload;
  int value;
  const char *name;
};

Project g_projects[] = {
  { .workload = 10, .value = 20, .name = "A" },
  { .workload = 20, .value = 30, .name = "B" },
  { .workload = 30, .value = 66, .name = "C" },
  { .workload = 40, .value = 20, .name = "D" },
  { .workload = 50, .value = 60, .name = "E" },
  { .workload = 60, .value = 50, .name = "F" },
};

struct Node
{
  Node(Project *p = nullptr) :
    project(p),
    connections() {}

  //attributes
  Project *project;
  vector<Node*> connections;
};

Node* buildGraph(Project projects[])
{
  Node* root = new Node();
  // Create corresponding nodes for each project entry
  for (int i = 0; i < PROJ_AMOUNT; ++i) {
    root->connections.push_back(new Node(projects + i));
  }

  // Connect nodes with each other, representing the
  // possible combinations among all the project entries.
  // Each project can be combined with all the subsequent
  // projects in the list
  auto end = root->connections.end();
  for (auto source = root->connections.begin(); source != end; ++source) {
    TRACE("%s:", (*source)->project->name);
    for (auto target = source+1; target != end; ++target) {
      (*source)->connections.push_back(*target);
      TRACE(" %s", (*target)->project->name);
    }
    TRACE("\n");
  }

  return root;
}

struct Combination {
  Combination() :
    value(0),
    workload(0),
    projects() {}

  int depth() const
  {
    return (projects.size() == 0) ? 0 :
      projects.back() - g_projects + 1;
  }

  void push(Project *p) {
    assert(p != nullptr);
    value += p->value;
    workload += p->workload;
    projects.push_back(p);
  }

  void pop() {
    if (projects.size() == 0)
      return;
    Project *p = projects.back();
    value -= p->value;
    workload -= p->workload;
    projects.pop_back();
  }

  std::string toString()
  {
    std::ostringstream stream;
    stream << "(workload=" << std::setw(3) << workload
      << " value=" << std::setw(3) << value << ") -> [";
    for (auto& p: projects)
      stream << " " << p->name;
    stream << " ]";
    return stream.str();
  }

  int value;
  int workload;
  vector<Project *> projects;
};

// Used C++ non-const references here instead of local
// instances + returning Combination object just to save
// some allocations and copy constructions. A single
// Combination is used to generate (push/pop) all combinations
// and result ref is used to store the current combination
// with optimal value/workload combination, which is used
// in the end as the and result of the algorithm.
void checkCombinations(Combination &curr, Combination &result)
{
  if (curr.workload > MAX_WORKLOAD)
    return;

  int depth = curr.depth();
  if (depth > 0) {
    TRACE("%s() depth=%d %s\n", __FUNCTION__,
        depth, curr.toString().c_str());
  }
  if (curr.value > result.value)
    result = curr;

  for (int i = depth; i < PROJ_AMOUNT; ++i) {
    curr.push(&g_projects[i]);
    checkCombinations(curr, result);
    curr.pop();
  }
}

int main()
{
#if 0
  Combination comb, result;
  checkCombinations(comb, result);
  printf("\n\n -----> Optimal choice: %s\n", result.toString().c_str());
#else
  Node *root = buildGraph(g_projects);
  delete root;
#endif
  return 0;
}

