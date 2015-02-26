do_insert(X, Y, [X|Y]).
do_insert(X, [Y1|Y2], [Y1|Z]) :-
  do_insert(X, Y2, Z).

permute([X], [X]).

permute([X|Y], Z) :-
  permute(Y, Z1),
  do_insert(X, Z1, Z).