can_place(_, []) :- !.
can_place(q(R, C), Board) :-
  member(q(Ra, Ca), Board),
  abs(Ra-R) =:= abs(Ca-C),
  !, fail.
can_place(_, _).

queens([], [], Board, Board).
queens([q(R)|Queens], Columns, Board, Solution) :-
  nth0(_, Columns, C, Free),
  can_place(q(R, C), Board),
  queens(Queens, Free, [q(R, C)|Board], Solution).

queens :-
  findall(q(N), between(0,3, N), Queens), % Queens = [q(0), q(1), q(2), q(3), q(4), q(5), q(6), q(7)].
  findall(N, between(0, 3, N), Columns), % Columns = [0, 1, 2, 3, 4, 5, 6, 7].
  findall(B, queens(Queens, Columns, [], B), Boards),
  length(Boards, Len),
  writef('%w solutions:\n', [Len]),
  member(R, Boards),
  reverse(R, Board),
  findall(C, nth0(_, Board, q(R1, C), Fr), Rfin),
  writef('  - %w\n', [Rfin]),
  fail.
queens.