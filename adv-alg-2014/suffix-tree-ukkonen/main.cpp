#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

unsigned char s[250001];
int c = 0;

struct Node;

struct Edge
{
  Node* node;
  int i0;
  int i1;

  int i1Real()
  {
    return i1 == -1 ? c : i1;
  }

  Edge(int i0, int i1)
    : i0(i0)
    , i1(i1)
    , node(NULL)
  {

  }
};

struct Node
{
  Edge* edges[256];
  Node* suffixLink;

  Node()
    : suffixLink(NULL)
  {
    fill(edges, edges + 256, static_cast<Edge*>(NULL));
  }
};

Node root;
Node* activeNode = &root;
unsigned char activeEdge = 0;
int activeLength = 0;
int debt = 0;

void Print(Node* node, int l = 0)
{
  if (node == NULL)
  {
    return;
  }
  for (int i = 0; i < 256; i++)
  {
    if (node->edges[i] != NULL)
    {
      Edge& e = *node->edges[i];
      int i1 = e.i1;
      if (i1 == -1)
      {
        i1 = c;
      }
      for (int j = 0; j < l; j++)
      {
        printf("  ");
      }
      printf("%d %d : %c : ", e.i0, e.i1, i);
      for (int j = e.i0; j <= i1; j++)
      {
        printf("%c", s[j]);
      }
      printf("\n");
      Print(e.node, l + 1);
    }
  }
  if (l == 0)
  {
    printf("at c = %d\n", c);
    printf("================================================================================\n");
  }
}

void PushLetter(unsigned char newLetter)
{
  unsigned char presentLetter = 0;
  if (activeEdge != 0)
  {
    int i0 = activeNode->edges[activeEdge]->i0;
    int i1 = activeNode->edges[activeEdge]->i1Real();
    if (i1 < i0 + activeLength + 1)
    {
      if (activeNode->edges[activeEdge]->node->edges[newLetter])
      {
        activeNode = activeNode->edges[activeEdge]->node;
        activeEdge = newLetter;
        activeLength = -1;
        presentLetter = newLetter;
      }
    }
    else
    {
      presentLetter = s[activeNode->edges[activeEdge]->i0 + activeLength + 1];
    }

  }
  else if (activeNode->edges[newLetter] != NULL)
  {
    presentLetter = newLetter;
  }

  Node* lastMadeNode = NULL;

  if (presentLetter != newLetter)
  {
    while (debt > 0)
    {
      Edge& e = *activeNode->edges[activeEdge];
      int i1 = e.i1;
      e.i1 = e.i0 + activeLength;
      Node* tempNode = e.node;
      if (e.i1 != i1)
      {
        e.node = new Node();
        e.node->edges[presentLetter] = new Edge(e.i0 + activeLength + 1, i1);
        e.node->edges[presentLetter]->node = tempNode;
      }
      e.node->edges[newLetter] = new Edge(c, -1);
      debt--;

      if (lastMadeNode != NULL)
      {
        lastMadeNode->suffixLink = e.node;
      }
      lastMadeNode = e.node;

      if (activeNode == &root)
      {
        activeEdge = s[c - debt];
        activeLength--;
      }
      else
      {
        if (activeNode->suffixLink == 0)
        {
          activeNode = &root;
        }
        else
        {
          activeNode = activeNode->suffixLink;
        }
        {
          int i0 = activeNode->edges[activeEdge]->i0;
          int i1 = activeNode->edges[activeEdge]->i1;
          if (i1 == -1)
          {
            i1 = c;
          }
          while (activeNode->edges[activeEdge]->i0 + activeLength > i1)
          {
            if (activeNode->edges[activeEdge]->node != NULL)
            {
              activeNode = activeNode->edges[activeEdge]->node;
              activeEdge = s[c - debt + i1 - i0 + 1];
              activeLength -= (i1 - i0 + 1);
            }
            if (!activeNode->edges[activeEdge])
            {
              activeNode->edges[activeEdge] = new Edge(c - debt + i1 - i0 + 1, -1);
            }
            i0 = activeNode->edges[activeEdge]->i0;
            i1 = activeNode->edges[activeEdge]->i1;
            if (i1 == -1)
            {
              i1 = c;
            }
          }
        }
      }
    }
    if (activeNode->edges[newLetter] == NULL)
    {
      activeEdge = 0;
      activeLength = 0;
      activeNode->edges[newLetter] = new Edge(c, -1);
    }
  }

  if (presentLetter == newLetter)
  {
    if (activeEdge == 0)
    {
      activeEdge = newLetter;
    }
    else
    {
      activeLength++;
    }
    debt++;

    int i0 = activeNode->edges[activeEdge]->i0;
    int i1 = activeNode->edges[activeEdge]->i1;
    if (i1 == -1)
    {
      i1 = c;
    }
    while (activeNode->edges[activeEdge]->i0 + activeLength > i1)
    {
      if (activeNode->edges[activeEdge]->node != NULL)
      {
        activeNode = activeNode->edges[activeEdge]->node;
        activeEdge = s[i1 + 1];
        activeLength -= (i1 - i0 + 1);
      }
      i0 = activeNode->edges[activeEdge]->i0;
      i1 = activeNode->edges[activeEdge]->i1;
      if (i1 == -1)
      {
        i1 = c;
      }
    }
  }
}

bool CheckWord(unsigned char* word)
{
  Node* node = &root;
  int i = 0;
  unsigned char letter = word[i];
  while (letter != 0)
  {
    if (node->edges[letter] != NULL)
    {
      Edge& e = *node->edges[letter];
      for (int j = e.i0; j <= e.i1Real(); j++)
      {
        if (word[i] == 0)
        {
          return 1;
        }
        if (s[j] != word[i])
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
    node = node->edges[letter]->node;
    letter = word[i];
    if (node == NULL
        && letter != 0)
    {
      return 0;
    }
  }
  return 1;
}

int main()
{
  FILE* fi = fopen("_input.txt", "r");
  fscanf(fi, "%s", s);

  while (s[c] != 0)
  {
    PushLetter(s[c]);
//    Print(&root);
    c++;
  }

  int n;
  fscanf(fi, "%d", &n);
  unsigned char word[1001];

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
