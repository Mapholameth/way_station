#include <cstdlib>
#include <iostream>
#include <functional>
#include <ctime>
#include <iostream>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include <cmath>

#include "mpi.h"

using namespace std;

static const unsigned n = 1000000;

int main(int argc, char *argv[])
{
  int my_rank;
  int proc_count;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  clock_t time_begin = clock();

  double this_pi = 0.0;
  double comm_pi = 0.0;

  double step = 1.0 / n;
  double x;

  for (int i = my_rank; i < n; i += proc_count)
  {
    x = (i + 0.5) * step;
    this_pi += 4.0 / (1.0 + x * x);
  }
  this_pi *= step;

  MPI_Reduce(&this_pi, &comm_pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (my_rank == 0)
  {
    std::cout << "elapsed " << double(clock() - time_begin) / CLOCKS_PER_SEC * 1000
              << " ms " << std::endl;
  }

  MPI_Finalize();

  if (my_rank == 0)
  {
    printf("  my pi: %1.15f\n", comm_pi);
    printf("real pi: %1.15f\n", 3.141592653589793);
  }

  return 0;
}
