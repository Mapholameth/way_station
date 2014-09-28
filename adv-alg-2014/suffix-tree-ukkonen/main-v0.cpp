#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

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
  //Edge* edges[256];
  map<unsigned char, Edge*> edges;
  Node* suffixLink;

  Node()
    : suffixLink(NULL)
  {
    //fill(edges, edges + 256, static_cast<Edge*>(NULL));
  }
};

Node root;
Node* activeNode = &root;
unsigned char activeEdge = 0;
int activeLength = 0;
int debt = 0;
Node* lastMadeNode = NULL;

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

  if (presentLetter != newLetter)
  {
    while (debt > 0)
    {
      Edge& e = *activeNode->edges[activeEdge];

      if (e.node && e.node->edges[newLetter] != NULL)
      {
//        if (lastMadeNode != NULL)
//        {
//          lastMadeNode->suffixLink = e.node;
//          lastMadeNode = e.node;
//        }
//        break;
          printf("cyka ");
      }

      int i1 = e.i1;
      e.i1 = e.i0 + activeLength;
      Node* tempNode = e.node;
      if (e.i1 != i1)
      {
        e.node = new Node();
        unsigned char letter = presentLetter;
        if (s[activeNode->edges[activeEdge]->i0 + activeLength + 1] != presentLetter)
        {
          printf("blyad");
          letter = s[activeNode->edges[activeEdge]->i0 + activeLength + 1];
        }
        e.node->edges[letter] = new Edge(e.i0 + activeLength + 1, i1);
        e.node->edges[letter]->node = tempNode;
      }

      bool doRun = false;
      if (e.node->edges[newLetter] == NULL)
      {
        // .count(
        e.node->edges[newLetter] = new Edge(c, -1);
        debt--;
      }
      else
      {
        doRun = true;
      }


      if (lastMadeNode != NULL)
      {
        if (lastMadeNode->suffixLink != NULL)
        {
//          printf("pizdec");
        }
        else
        {
          lastMadeNode->suffixLink = e.node;
        }
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
      }

      if (doRun)
      {
        break;
      }

      // correct translation to right point
      if (activeLength != -1)
      {
        if (!activeNode->edges[activeEdge])
        {
          activeNode->edges[activeEdge] = new Edge(s[c - debt], -1);
        }

        int i0 = activeNode->edges[activeEdge]->i0;
        int i1 = activeNode->edges[activeEdge]->i1Real();

        while (activeNode->edges[activeEdge]->i0 + activeLength > i1)
        {
          if (activeNode->edges[activeEdge]->node != NULL)
          {
            activeNode = activeNode->edges[activeEdge]->node;
            Node* node = &root;
            int l = 0;
            int j = c - debt;
            while (node != activeNode)
            {
              Edge& e = * node->edges[s[j]];
              int d = e.i1Real() - e.i0 + 1;
              l += d;
              j += d;
              node = e.node;
              if (node == NULL)
              {
                printf("ebana");
              }
            }
            activeEdge = s[c - debt + l];
            activeLength -= (i1 - i0 + 1);
          }
          if (!activeNode->edges[activeEdge])
          {
            activeNode->edges[activeEdge] = new Edge(c - debt + i1 - i0 + 1, -1);
          }
          i0 = activeNode->edges[activeEdge]->i0;
          i1 = activeNode->edges[activeEdge]->i1Real();
        }
      }

    }
    if (activeNode->edges[newLetter] == NULL)
    {
      activeEdge = 0;
      activeLength = 0;
      activeNode->edges[newLetter] = new Edge(c, -1);
    }
    else
    {
      presentLetter = newLetter;
      if (lastMadeNode != NULL)
      {
        lastMadeNode->suffixLink = activeNode;
      }
    }
    lastMadeNode = NULL;
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
    int i1 = activeNode->edges[activeEdge]->i1Real();
    while (activeNode->edges[activeEdge]->i0 + activeLength > i1)
    {
      if (activeNode->edges[activeEdge]->node != NULL)
      {
        activeNode = activeNode->edges[activeEdge]->node;
        activeEdge = s[i1 + 1];
        activeLength -= (i1 - i0 + 1);
      }
      i0 = activeNode->edges[activeEdge]->i0;
      i1 = activeNode->edges[activeEdge]->i1Real();
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

//string s;
int n;

struct node {
  int l, r, par, link;
  map<char,int> next;

  node (int l=0, int r=0, int par=-1)
    : l(l), r(r), par(par), link(-1) {}
  int len()  {  return r - l;  }
  int &get (char c) {
    if (!next.count(c))  next[c] = -1;
    return next[c];
  }
};
node t[250001];
int sz;

struct state {
  int v, pos;
  state (int v, int pos) : v(v), pos(pos)  {}
};
state ptr (0, 0);

state go (state st, int l, int r) {
  while (l < r)
    if (st.pos == t[st.v].len()) {
      st = state (t[st.v].get( s[l] ), 0);
      if (st.v == -1)  return st;
    }
    else {
      if (s[ t[st.v].l + st.pos ] != s[l])
        return state (-1, -1);
      if (r-l < t[st.v].len() - st.pos)
        return state (st.v, st.pos + r-l);
      l += t[st.v].len() - st.pos;
      st.pos = t[st.v].len();
    }
  return st;
}

int split (state st) {
  if (st.pos == t[st.v].len())
    return st.v;
  if (st.pos == 0)
    return t[st.v].par;
  node v = t[st.v];
  int id = sz++;
  t[id] = node (v.l, v.l+st.pos, v.par);
  t[v.par].get( s[v.l] ) = id;
  t[id].get( s[v.l+st.pos] ) = st.v;
  t[st.v].par = id;
  t[st.v].l += st.pos;
  return id;
}

int get_link (int v) {
  if (t[v].link != -1)  return t[v].link;
  if (t[v].par == -1)  return 0;
  int to = get_link (t[v].par);
  return t[v].link = split (go (state(to,t[to].len()), t[v].l + (t[v].par==0), t[v].r));
}

void tree_extend (int pos) {
  for(;;) {
    state nptr = go (ptr, pos, pos+1);
    if (nptr.v != -1) {
      ptr = nptr;
      return;
    }

    int mid = split (ptr);
    int leaf = sz++;
    t[leaf] = node (pos, n, mid);
    t[mid].get( s[pos] ) = leaf;

    ptr.v = get_link (mid);
    ptr.pos = t[ptr.v].len();
    if (!mid)  break;
  }
}

void build_tree() {
  sz = 1;
  for (int i=0; i<n; ++i)
    tree_extend (i);
}

int needSL = 0;
int active_e = 0;


void PushLetter_v2(unsigned char newLetter)
{
  needSL = 0;
  debt++;
  while (debt > 0)
  {
    if (activeLength == 0)
      active_e = c;
    if (activeNode->edges[s[active_e]] == 0)
    {
      int leaf = new_node(c);
      activeNode->edges[s[active_e]] = leaf;
      add_SL(activeNode);
    }
    else
    {
      Node* nxt = activeNode->edges[s[active_e]];
      if (walk_down(nxt))
        continue;
      if (s[nxt->])
    }
    debt--;
    if (activeNode == &root && activeLength > 0)
    {
      activeLength--;
      active_e = c - debt + 1;
    }
    else
    {
      activeNode = activeNode->suffixLink;
      if (activeNode == NULL)
      {
        activeNode = &root;
      }
    }
  }
}

int main()
{
  FILE* fi = fopen("input.txt", "r");
  fscanf(fi, "%s", s);

//  n = strlen(reinterpret_cast<char*>(s));
//  build_tree();

//  while (true)
//  {
//    int endpes = 16;
//    for (int i = 0; i < endpes; i++)
//    {
//      s[i] = rand() % (5) + 'a';
//      if (i == endpes - 1)
//      {
//        s[i] = 0;
//      }
//    }
////    printf("%s\n", s);
    while (s[c] != 0)
    {
      PushLetter_v2(s[c]);
      Print(&root);
      c++;
    }
//    c = 0;
//    activeEdge = 0;
//    activeLength = 0;
//    activeNode = &root;
//    root.suffixLink = NULL;
//    debt = 0;
//    for (int i = 0; i < 256; i++)
//    {
//      root.edges[i] = NULL;
//    }
//  }

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
