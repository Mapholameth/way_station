#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <type_traits>
#include <vector>
#include <stack>

#include "mpi.h"

using namespace std;

struct Node
{
  double value;
  std::string text;
  std::vector<int> children;

  Node& child(int i);
  void add_child(int i);
};

static const unsigned MAX_NODES = 10000000;
unsigned nodesCount = 0;
Node nodes[MAX_NODES];

int Parse(char* s);
int MakeNode(double value);
int MakeNode(std::string text);
void PrintAST(Node& root, int indentLevel);
double Eval(int n_i);

// NOTE: `*` 120 times slower than `+`, `-`
// `/` 2 times slower than `*`

int main(int argc, char *argv[])
{
  int my_rank;
  int proc_count;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  int root = 0;

  if (my_rank == 0)
  {
    vector<char> s;
    FILE* fi = fopen("input.txt", "r");
    int c = 0;
    do
    {
      c = fgetc(fi);
      s.push_back(c);
//      printf("%c", c);
    } while (c != EOF);
    fclose(fi);
    s.push_back(0);
    root = Parse(s.data());
    printf("%d\n", nodesCount);
    flush(std::cout);
  }

  if (nodesCount < 128
      && my_rank == 0)
  {
    printf("\n\n");
    PrintAST(nodes[root], 4);
    printf("\n\n");
  }

  clock_t time_begin = clock();

  if (my_rank == 0)
  {
    printf("result: %f\n\n", Eval(root));
    std::cout
      << "elapsed "
      << double(clock() - time_begin)
         / CLOCKS_PER_SEC * 1000
      << " ms "
      << std::endl;
  }

  MPI_Finalize();

  return 0;
}

/** grammar:
 *
 * expression: term | term (`+`|`-`) expression
 * term: factor | term (`*`|`/`) factor
 * factor: number | `(` expression `)`
 */

bool Skip(char*& s)
{
  char c = s[0];
  if (c == 0)
  {
    return true;
  }
  else if (c == ' ' || c == '\r'
        || c == '\n' || c == '\t'
        || c == EOF)
  {
    s++;
    return Skip(s);
  }
  else
  {
    return false;
  }
}

int ParseFactor(char *&s);

int ParseTerm(char*& s)
{
  int lhs_i = ParseFactor(s);
  if (Skip(s))
  {
    return lhs_i;
  }
  char c = s[0];
  if (c != '*' && c != '/')
  {
    return lhs_i;
  }
  int op_i = MakeNode(std::string() + c);
  nodes[op_i].add_child(lhs_i);
  while (s[0] == c)
  {
    s++;
    int rhs = ParseFactor(s);
    nodes[op_i].add_child(rhs);
    if (Skip(s))
    {
      return op_i;
    }
    else if (s[0] != c
             && (s[0] == '*' || s[0] == '/'))
    {
      int op_new_i = MakeNode(std::string() + s[0]);
      nodes[op_new_i].add_child(op_i);
      op_i = op_new_i;
      c = s[0];
    }
  }
  return op_i;
}

int ParseExpression(char*& s)
{
  int lhs_i = ParseTerm(s);
  if (Skip(s))
  {
    return lhs_i;
  }
  char c = s[0];
  if (c != '+' && c != '-')
  {
    return lhs_i;
  }
  int op_i = MakeNode(std::string() + c);
  nodes[op_i].add_child(lhs_i);
  while (s[0] == c)
  {
    s++;
    int rhs = ParseTerm(s);
    nodes[op_i].add_child(rhs);
    if (Skip(s))
    {
      return op_i;
    }
    else if (s[0] != c
             && (s[0] == '+' || s[0] == '-'))
    {
      int op_new_i = MakeNode(std::string() + s[0]);
      nodes[op_new_i].add_child(op_i);
      op_i = op_new_i;
      c = s[0];
    }
  }
  return op_i;
}

int ParseFactor(char*& s)
{
  if (Skip(s))
  {
    return -1;
  }

  char c = s[0];
  int r_i = -1;
  if (c == '(')
  {
    s++;
    r_i = ParseExpression(s);
    if (s[0] != ')')
    {
      throw "error: missing `)`";
    }
    s++;
  }
  else if ('0' <= c && c <= '9')
  {
    char* s_end = nullptr;
    double value = strtod(s, &s_end);
    // TODO: check errno
    s = s_end;
    r_i = MakeNode(value);
  }
  return r_i;
}

int Parse(char* s)
{
  char* tempStrPtr = s;
  return ParseExpression(s);
}

void PrintAST(Node& root, int indentLevel)
{
  auto print = [&]() -> decltype(std::operator <<(std::cout, std::string()))
  {
      return std::cout << std::string(indentLevel * 2, ' ');
  };

  struct PrintTreeNode
  {
    std::string text;
    std::vector<PrintTreeNode*> children;
    unsigned depth = 0;
  };

  std::vector<int> offsetByDepth;
  offsetByDepth.resize(1);
  offsetByDepth[0] = 0;
  unsigned depth = 0;
  const int spacing = 2;

  std::function<void(Node&, PrintTreeNode*, int)> f =
      [&](Node& next, PrintTreeNode* print, int estimate)
  {
    if (offsetByDepth.size() < depth + 1)
    {
      offsetByDepth.resize(depth + 1, 0);
    }

    int spaceCount = estimate - offsetByDepth[depth];
    spaceCount = spaceCount == 1 ? 2 : spaceCount;

    if (next.children.size() == 0)
    {
      assert(print != NULL);
      print->text = std::string(spaceCount, ' ') + next.text;
      print->depth = depth;
      offsetByDepth[depth] += next.text.size() + spaceCount;
    }
    else
    {
      int maxLeft = -1;
      int prevMaxLeft = -1;
      for (int i = 0; i < next.children.size(); i++)
      {
        PrintTreeNode* printNode = new PrintTreeNode;
        print->children.push_back(printNode);

        depth++;
        f(next.child(i), printNode, (maxLeft + spacing) * (i != 0)
          + estimate * (i == 0));
        prevMaxLeft = maxLeft;
        maxLeft = *std::max_element(offsetByDepth.begin() + depth,
                                    offsetByDepth.end());
        depth--;
      }

      print->depth = depth;

      if (prevMaxLeft != -1)
      {
        int decorationCount = prevMaxLeft + spacing - estimate
            - (next.text.size() - 1);

        print->text = std::string(spaceCount, ' ')
            + next.text
            + std::string(decorationCount, '-');

        offsetByDepth[depth] += next.text.size() + prevMaxLeft
            + spacing - estimate + spaceCount - (next.text.size() - 1);
      }
      else
      {
        print->text = std::string(spaceCount, ' ')
            + next.text;
        offsetByDepth[depth] += next.text.size() + spaceCount;
      }
    }
  };

  PrintTreeNode printRoot;
  f(root, &printRoot, 0);

  std::queue<PrintTreeNode*> queue;
  queue.push(&printRoot);
  vector<PrintTreeNode*> line;
  depth = 0;

  print();
  while (!queue.empty())
  {
    PrintTreeNode* next = queue.front();
    queue.pop();

    for (size_t i = 0; i < next->children.size(); i++)
    {
      queue.push(next->children[i]);
    }

    std::cout << next->text;
    line.push_back(next);

    if (!queue.empty() && depth != queue.front()->depth)
    {
      std::cout << std::endl;
      depth = queue.front()->depth;
      print();
      for (auto node : line)
      {
        for (size_t i = 0; i < node->children.size(); i++)
        {
          std::string& text = node->children[i]->text;
          auto leadingSpaceCount = text.find_first_not_of(' ');
          std::cout << std::string(leadingSpaceCount, ' ');
          std::cout << "|";
          std::cout << std::string(text.size() - leadingSpaceCount - 1, ' ');
        }
      }
      std::cout << std::endl;
      print();
      line.clear();
    }
  }
}


Node& Node::child(int i)
{
  return nodes[children[i]];
}

void Node::add_child(int i)
{
  children.push_back(i);
}

int MakeNode(std::string text)
{
  nodes[nodesCount].text = text;
  nodesCount++;
  return nodesCount - 1;
}

int MakeNode(double value)
{
  nodes[nodesCount].value = value;
  nodes[nodesCount].text = to_string(static_cast<int>(value)); // double's too long
  nodesCount++;
  return nodesCount - 1;
}

double Eval(int n_i)
{
  Node& n_n = nodes[n_i];
  char c = n_n.text[0];
  double t = 0.0f;
  if (n_n.children.size() > 0)
  {
    t = Eval(n_n.children[0]);
  }
  switch(c)
  {
    case '+':
      for (int i = 1; i < n_n.children.size(); i++)
      {
        t += Eval(n_n.children[i]);
      }
      break;

    case '-':
      for (int i = 1; i < n_n.children.size(); i++)
      {
        t -= Eval(n_n.children[i]);
      }
      break;

    case '*':
      for (int i = 1; i < n_n.children.size(); i++)
      {
        t *= Eval(n_n.children[i]);
      }
      break;

    case '/':
      for (int i = 1; i < n_n.children.size(); i++)
      {
        t /= Eval(n_n.children[i]);
      }
      break;

    default:
      return n_n.value;
  }
  return t;
}
