#include <cstdio>
#include <string>
#include <map>
#include <limits>

const int oo = std::numeric_limits<int>::max();
const int MAXN = 250000;

using namespace std;

int root = 0;
int lastAdded = 0;
int pos = 0;
int needSL = 0;
int remainder = 0;
int activeNode = 0;
int activeEdge = 0;
int activeLength = 0;
char text[MAXN];

struct Node
{
  // [start; end)
  int start;
  int end;
  int slink;
  map<unsigned char, int> next;

  int Length()
  {
    return min(end, pos + 1) - start;
  }
};

Node tree[MAXN * 2];

int MakeNewNode(int start, int end = oo)
{
  Node nd;
  nd.start = start;
  nd.end = end;
  nd.slink = 0;
  tree[++lastAdded] = nd;
  return lastAdded;
}

char GetActiveEdge()
{
  return text[activeEdge];
}

void AddSuffixLink(int node)
{
  if (needSL > 0)
  {
    tree[needSL].slink = node;
  }
  needSL = node;
}

bool WalkDown(int node)
{
  if (activeLength >= tree[node].Length())
  {
    activeEdge += tree[node].Length();
    activeLength -= tree[node].Length();
    activeNode = node;
    return true;
  }
  else
  {
    return false;
  }
}

void PushLetter()
{
  needSL = 0;
  remainder++;

  while(remainder > 0)
  {
    if (activeLength == 0)
    {
      activeEdge = pos;
    }

    if (tree[activeNode].next.count(GetActiveEdge()) == 0)
    {
      int leaf = MakeNewNode(pos);
      tree[activeNode].next[GetActiveEdge()] = leaf;
      AddSuffixLink(activeNode);
    }
    else
    {
      int next = tree[activeNode].next[GetActiveEdge()];
      if (WalkDown(next))
      {
        continue;
      }

      if (text[tree[next].start + activeLength] == text[pos])
      {
        activeLength++;
        AddSuffixLink(activeNode);
        break;
      }

      int split = MakeNewNode(tree[next].start, tree[next].start + activeLength);
      tree[activeNode].next[GetActiveEdge()] = split;
      int leaf = MakeNewNode(pos);
      tree[split].next[text[pos]] = leaf;
      tree[next].start += activeLength;
      tree[split].next[text[tree[next].start]] = next;
      AddSuffixLink(split);
    }
    remainder--;

    if (activeNode == root
        && activeLength > 0)
    {
      activeLength--;
      activeEdge = pos - remainder + 1;
    }
    else
    {
      activeNode = tree[activeNode].slink > 0 ? tree[activeNode].slink : root;
    }
  }
}

void Print(int node_id, int l = 0)
{
  for (int i = 0; i < 256; i++)
  {
    if (tree[node_id].next[i] != 0)
    {
      Node e = tree[tree[node_id].next[i]];
      for (int j = 0; j < l; j++)
      {
        printf("  ");
      }
      int end = e.end;
      end = end == oo ? pos + 1 : end;
      printf("%d %d : %c : ", e.start, end, i);
      for (int j = e.start; j < end; j++)
      {
        printf("%c", text[j]);
      }
      printf("\n");
      Print(tree[node_id].next[i], l + 1);
    }
  }
  if (l == 0)
  {
    printf("at c = %d\n", pos);
    printf("================================================================================\n");
  }
}

bool CheckWord(unsigned char* word)
{
  Node n = tree[root];
  int i = 0;
  unsigned char letter = word[i];
  while (letter != 0)
  {
    if (n.next.count(letter) != 0)
    {
      Node e = tree[n.next[letter]];
      int i0 = e.start;
      int i1 = e.end == oo ? pos + 1 : e.end;
      for (int j = i0; j < i1; j++)
      {
        if (word[i] == 0)
        {
          return 1;
        }
        if (text[j] != word[i])
        {
          return 0;
        }
        i++;
      }
    }
    else
    {
      return 0;
    }
    n = tree[n.next[letter]];
    letter = word[i];
    if (n.next[letter] == 0
        && letter != 0)
    {
      return 0;
    }
  }
  return 1;
}

int main()
{
  FILE* fi = fopen("input.txt", "r");
  fscanf(fi, "%s", text);

  root = activeNode = MakeNewNode(-1, -1);

  while (text[pos] != 0)
  {
    PushLetter();
//    Print(root);
    pos++;
  }

  int n;
  fscanf(fi, "%d", &n);
  unsigned char word[1000];

  FILE* fo = fopen("output.txt", "w");

  for (int i = 0; i < n; i++)
  {
    fscanf(fi, "%s", word);
    fprintf(fo, "%d ", CheckWord(word));
  }

  fclose(fi);
  fclose(fo);

  return 0;
}
