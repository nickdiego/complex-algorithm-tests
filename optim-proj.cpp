#include <list>
#include <sstream>
#include <iomanip>
#include <cstdio>
#include <cstdint>

#if 1
#define PROJ_AMOUNT 5
#define MAX_WORKLOAD 100
#else
#define PROJ_AMOUNT 6
#define 120
#endif

#if ENABLE_DEBUG
#define TRACE(...) fprintf(stdout, __VA_ARGS__)
#else
#define TRACE(...)
#endif

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

class Permuter
{
public:
  Permuter(Project p[], int projAmount, int maxWL) :
    m_projects(p),
    m_bits(0),
    m_perm(),
    kBitAmount(projAmount),
    kMaxWorkload(maxWL),
    kMaxBitValue(~((BitSet)~0 << kBitAmount))
  {
    TRACE("maxbitval=%lx\n", kMaxBitValue);
  }

  // Choose the best project`s permutation,
  // optimizing the `total value` delvered and
  // considering the max workload of the team.
  // Non-recursive -- Complexity = O(2^n)
  // Where N is the projects amount
  void process()
  {
    Permutation optimal;
    while (next()) {
      if (m_perm.workload > kMaxWorkload)
        continue;
      TRACE("%s() %s\n", __FUNCTION__, m_perm.toString().c_str());
      if (m_perm.value > optimal.value)
        optimal = m_perm;
    }
    printf("\n\n-----> Optimal choice: %s\n", optimal.toString().c_str());
  }

  bool next()
  {
    if (++m_bits > kMaxBitValue)
      return false;
    m_perm.workload = m_perm.value = 0;
    m_perm.projects.clear();
    for (int i = 0; i < kBitAmount; ++i) {
      if (m_bits & ((BitSet)1 << i)) {
        m_perm.workload += m_projects[i].workload;
        m_perm.value += m_projects[i].value;
        m_perm.projects.push_back(m_projects + i);
      }
    }
    return true;
  }

private:
  typedef uint64_t BitSet;
  struct Permutation {
    Permutation() :
      projects(), value(0), workload(0)
    {}
    std::list<Project*> projects;
    int value;
    int workload;
    std::string toString()
    {
      std::ostringstream pstr;
      pstr << "(workload=" << std::setw(3) << workload
        << " value=" << std::setw(3) << value << ") -> [";
      for (auto &p: projects) pstr << " " << p->name;
      pstr << " ]";
      return pstr.str();
    }
  };

private:
  Project *m_projects;
  BitSet m_bits;
  Permutation m_perm;

  const int kBitAmount;
  const int kMaxWorkload;
  const BitSet kMaxBitValue;
};

int main()
{
  Permuter permuter(g_projects, PROJ_AMOUNT, MAX_WORKLOAD);
  permuter.process();
  return 0;
}

