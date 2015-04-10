do_insert(X, Y, [X|Y]).
do_insert(X, [Y1|Y2], [Y1|Z]) :-
  do_insert(X, Y2, Z).

permute([X], [X]).

permute([X|Y], Z) :-
  permute(Y, Z1),
  do_insert(X, Z1, Z).

test_perm :-
  findall(R, permute([1,2,3,4], R), Y),
  length(Y, Len),
  writef('%w solutions:\n', [Len]),
  writef('  - %w\n', [Y]),
  fail.
test_perm.
