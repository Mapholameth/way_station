// MS MPI with MinGW results in: (thank god it's only this)
typedef long long int __int64;

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

static const unsigned m_size = 1024;

void fill_matrix(double m[m_size][m_size], std::function<double(double, double)> f)
{
  for (unsigned i = 0; i < m_size; i++)
  {
    for (unsigned j = 0; j < m_size; j++)
    {
      m[i][j] = f(i, j);
    }
  }
}

void print_matrix(double m[m_size][m_size])
{
  for (unsigned i = 0; i < m_size; i++)
  {
    printf("\n\t| ");
    for (unsigned j = 0; j < m_size; j++)
    {
      printf("%f ", m[i][j]);
    }
    printf("|");
  }
}

double A[m_size][m_size];
double B[m_size][m_size];
double C[m_size][m_size];

int main(int argc, char *argv[])
{
  int my_rank;
  int proc_count;
  int from;
  int to;
  int i;
  int j;
  int k;

  MPI_Init (&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  from = (my_rank + 0) * m_size / proc_count;
  to = (my_rank + 1) * m_size / proc_count;

  if (my_rank == 0)
  {
    static int n = 0;
    fill_matrix(A, [&](double i, double j){ return n++; });
    fill_matrix(B, [&](double i, double j){ return 1; });
    fill_matrix(C, [](double, double){ return -1.0; });
  }

  clock_t time_begin = clock();

  MPI_Bcast(B, m_size * m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(A, m_size * m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  double pes[m_size];
  printf("computing slice %d (from row %d to %d)\n", my_rank, from, to - 1);
  for (i = from; i < to; i++)
  {
    for (j = 0; j < m_size; j++)
    {
      C[i][j] = 0.0;
      for (k = 0; k < m_size; k++)
      {
        // B[j][k] assuming B is really B transposed
        // this makes calcs CPU cache aware and x10 perf gain
        C[i][j] += A[i][k] * B[j][k];
      }
    }
  }

  if (my_rank == 0)
  {
    MPI_Gather(MPI_IN_PLACE, m_size * m_size / proc_count, MPI_DOUBLE,
               C, m_size * m_size / proc_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }
  else
  {
    MPI_Gather(C + from, m_size * m_size / proc_count, MPI_DOUBLE,
               C, m_size * m_size / proc_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  }

  if (my_rank == 0)
  {
    std::cout << "elapsed " << double(clock() - time_begin) / CLOCKS_PER_SEC * 1000
              << " ms " << std::endl;
  }

  MPI_Finalize();

  if (my_rank == 0
      && m_size <= 12)
  {
    printf("\n\n");
    print_matrix(A);
    printf("\n\n\t       *\n");
    print_matrix(B);
    printf("\n\n\t       =\n");
    print_matrix(C);
    printf("\n\n");
  }

  return 0;
}
