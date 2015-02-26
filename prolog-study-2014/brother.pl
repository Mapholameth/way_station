solve(true, 1.0) :- !.
solve(A, CF) :-
  clause_of(A, B, CF1),
  solve(B, CF2),
  CF is CF1 * CF2.

solve((B1, B2), CF) :-
  solve(B1, CF1),
  solve(B2, CF2),
  min(CF1, CF2, CF).

min(CF1, CF2, CF) :- CF1 < CF2, CF is CF1, !.
min(CF1, CF2, CF) :- CF1 >= CF2, CF is CF2.

clause_of(, B, 0.5)