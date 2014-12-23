/** grammar:
 * regex -> alternative
 * regex -> regex `|` alternative
 * alternative -> closure
 * alternative -> alternative closure
 * closure -> group
 * closure -> closure `*`
 * group -> letter
 * group -> `(` regex `)`
 * letter -> a..z
 * letter -> A..Z
 * letter -> 0..9
 */

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <memory>
#include <unordered_set>

using namespace std;

struct Node : public std::enable_shared_from_this<Node>
{
  typedef shared_ptr<Node> NodePtr;
  char letter;
  vector<NodePtr> next;
  NodePtr followup;
  NodePtr parent;

  NodePtr LastOf()
  {
    NodePtr last = shared_from_this();
    while (last->followup != nullptr)
    {
      last = last->followup;
    }
    return last;
  }

  bool PredictNext(char newLetter, unordered_set<NodePtr>& reachable, int lookAhead);

  Node(char letter);
};

typedef shared_ptr<Node> NodePtr;

NodePtr Parse(char* s);
bool ClassifyString(NodePtr root, char* s);

int main()
{
  FILE* fi = fopen("input.txt", "r");

  const int maxStringLength = 80;
  char regex[maxStringLength];
  char s[maxStringLength];
  int n = 0;
  int testCount = 0;
  int testMajorIdx = 1;
  int testMinorIdx = 0;
  int failedCount = 0;

  // warning: don't put eol at the end of input by accident
  while (!feof(fi))
  {
    fscanf(fi, "%s%d", regex, &n);

    NodePtr root = Parse(regex);

    int referenceAnswer;
    testMinorIdx = 1;

    for (int i = 0; i < n; i++)
    {
      fscanf(fi, "%d%s", &referenceAnswer, s);
      bool fail = ClassifyString(root, s) != referenceAnswer;
      if (fail)
      {
        failedCount++;
        printf("fail: test %d.%d (%d): %s - %s : %d\n", testMajorIdx
             , testMinorIdx, testCount, regex, s, referenceAnswer);
      }
//      else printf("win: %s - %s : %d\n", regex, s, referenceAnswer);
      testMinorIdx++;
      testCount++;
    }

    testMajorIdx++;
  }

  fclose(fi);

  printf("test count: %d, failed %d\n", testCount, failedCount);

  return 0;
}

NodePtr ParseRegex(char*& s);
NodePtr ParseAlternative(char*& s);
NodePtr ParseClosure(char*& s);
NodePtr ParseGroup(char*& s);

NodePtr Parse(char* s)
{
  char* tempStrPtr = s;
  NodePtr r = make_shared<Node>(1);
  r->followup = ParseRegex(tempStrPtr);
  return r;
}

NodePtr ParseRegex(char*& s)
{
  NodePtr r = ParseAlternative(s);
  while (s[0] == '|')
  {
    if (r->letter != '|')
    {
      NodePtr temp = make_shared<Node>('|');
      temp->next.push_back(r);
      r = temp;
    }

    s++;
    r->next.push_back(ParseRegex(s));
  }
  return r;
}

NodePtr ParseAlternative(char*& s)
{
  NodePtr last = nullptr;
  NodePtr r = nullptr;
  while (isalnum(s[0])
         || s[0] == '(')
  {
    NodePtr temp = ParseClosure(s);
    if (last != nullptr)
    {
      last->followup = temp;
    }
    else
    {
      r = temp;
    }
    last = temp->LastOf();
  }
  return r;
}

NodePtr ParseClosure(char*& s)
{
  NodePtr r = ParseGroup(s);
  while (s[0] == '*')
  {
    s++;
    NodePtr closureNode = make_shared<Node>('*');
    closureNode->next.push_back(r);
    if (s[0] == 0)
    {
      closureNode->LastOf()->followup = make_shared<Node>(0);
    }
    r->parent = closureNode;
    r->LastOf()->followup = r;
    r = closureNode;
  }
  return r;
}

NodePtr ParseGroup(char*& s)
{
  char c = s[0];
  NodePtr temp = nullptr;

  if (isalnum(c))
  {
    s++;
    temp = make_shared<Node>(c);
  }
  else if (c == '(')
  {
    s++;
    temp = ParseRegex(s);
    if (s[0] != ')')
    {
      throw "error";
    }
    s++;
  }

  if (s[0] == 0)
  {
    temp->LastOf()->followup = make_shared<Node>(0);
  }
  return temp;
}

bool ClassifyString(NodePtr root, char* s)
{
  unordered_set<NodePtr> front;
  front.insert(root);
  char* sPtr = s;

  do
  {
    unordered_set<NodePtr> next;

    for (auto node : front)
    {
      if (isalnum(node->letter)
          && node->followup != nullptr)
      {
        node->followup->PredictNext(sPtr[0], next, 1);
      }
      else
      {
        node->PredictNext(sPtr[0], next, 1);
      }
    }
    sPtr++;
    front = next;
  } while (sPtr[0] != 0);

  unordered_set<NodePtr> next;
  for (auto node : front)
  {
    node->PredictNext(sPtr[0], next, 1);
  }
  sPtr++;
  front = next;

  return !front.empty();
}

bool Node::PredictNext(char newLetter
                     , unordered_set<NodePtr>& reachable
                     , int lookAhead)
{
  bool r = false;

  if ((isalnum(letter)
       || letter == 0)
      && newLetter == letter
      /*&& lookAhead == 0*/)
  {
    r = true;
    reachable.insert(shared_from_this());
  }
  else if (letter == '|')
  {
    for (int i = 0; i < next.size(); i++)
    {
      NodePtr node = next[i];
      r |= node->PredictNext(newLetter, reachable, lookAhead);
    }
  }
  else if (letter == '*')
  {
    NodePtr temp = shared_from_this();
    NodePtr start = temp;
    do
    {
      r = temp->next.front()->PredictNext(newLetter, reachable, lookAhead);
      temp = temp->followup;
    } while (!r
             && temp != nullptr
             && temp->letter == '*'
             && temp != start);

    if (!r
        && temp != nullptr
        && temp->letter != '*')
    {
      temp->PredictNext(newLetter, reachable, lookAhead - 1);
    }
  }

  if (r
      && followup != nullptr
      && followup->followup != nullptr
      && followup->followup->parent != nullptr
      && followup->followup->parent->letter == '*')
  {
    reachable.insert(followup->followup->parent);
  }

  if (followup != nullptr
      && lookAhead > 0)
  {
    lookAhead--;

    bool fr = followup->PredictNext(newLetter, reachable, lookAhead);
    if (fr)
    {
      //reachable.insert(followup);
    }
  }

  return r;
}

Node::Node(char letter)
  : letter(letter)
  , followup(nullptr)
  , parent(nullptr)
{

}
