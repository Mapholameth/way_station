%
dutch(L, RWB) :-
  distr_dl(L, dl(RWB, WB), dl(WB, B), dl(B, [])).

distr_dl([red(H)|T], dl(R, R1), W, B) :-
   distr_dl(T, dl(R, [red(H)|R1]), W, B).

distr_dl([white(H)|T], R, dl(W, W1), B) :-
  distr_dl(T, R, dl(W, [white(H)|W1]), B).

distr_dl([blue(H)|T], R, W, dl(B, B1)) :-
  distr_dl(T, R, W, dl(B, [blue(H)|B1])).

distr_dl([], dl(R, R), dl(W, W), dl(B, B)).

dl(A, B).

% dutch([red(1), white(2), blue(3), red(4), white(5)], RWB)?