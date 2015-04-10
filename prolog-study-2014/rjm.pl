maze_at(R, C, V) :-
  maze(M),
  nth0(R, M, T),
  nth0(C, T, V).

maze_length(L) :-
  maze(M),
  length(M, L).

maze(M) :-
G = -1,
M = [
  [3,4,1,3,1],
  [3,3,3,G,2],
  [3,1,2,2,3],
  [4,2,3,3,3],
  [4,1,4,3,2]
].

up.
down.
left.
right.

dir(go((RS, CS), (RD, CD)), DIR) :- RS = RD, CS > CD, DIR = left.
dir(go((RS, CS), (RD, CD)), DIR) :- RS = RD, CS < CD, DIR = right.
dir(go((RS, CS), (RD, CD)), DIR) :- CS = CD, RS > RD, DIR = up.
dir(go((RS, CS), (RD, CD)), DIR) :- CS = CD, RS < RD, DIR = down.

main :-
  maze(X),
  writef('maze: %w\n', [X]),
  bfs((0, 0), (1, 3), P),
  maplist(dir, P, R),
  writef('path found: %w\n', [R]).

step(A, B, R) :- R is A + B, R >= 0, maze_length(L), R < L.
step(A, B, R) :- R is A - B, R >= 0, maze_length(L), R < L.

st((RS, CS), TO, ST) :-
  maze_at(RS, CS, V),
  step(RS, V, RN),
  TO = (RN, CS),
  ST = go((RS, CS), TO).

st((RS, CS), TO, ST) :-
  maze_at(RS, CS, V),
  step(CS, V, CN),
  TO = (RS, CN),
  ST = go((RS, CS), TO).

bfs(X, Y, P) :-
  bfs_d(Y, [n(X, [])], [], R),
  reverse(R, P).

bfs_d(Y, [n(Y, P)|_], _, P).

bfs_d(Y, [n(S, P1)|Ns], C, P) :-
  length(P1, L),
  findall(n(S1, [A|P1]),
    (st(S, S1, A),
     \+ (member(n(S1, P2), Ns),
         length(P2, L)),
     \+ member(S1, C)),
    Es),
  append(Ns, Es, O),
  bfs_d(Y, O, [S|C], P).
