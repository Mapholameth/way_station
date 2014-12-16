#include <cstdlib>
#include <iostream>
#include <functional>
#include <ctime>
#include <limits>
#include <algorithm>
#include <type_traits>
#include <cassert>
#include <cmath>

#include "mpi.h"

using namespace std;

static const int m_size = 2048;

double A[m_size][m_size];
double b[m_size];
double c[m_size];
double x[m_size];
double map[m_size];

int main(int argc, char **argv)
{
  int i;
  int j;
  int k;
  double sum = 0.0;
  int my_rank;
  int proc_count;
  clock_t time_begin;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  if (my_rank == 0)
  {
    auto rand_range = [](){
      const double range = 1.0;
      return range * (1.0 - 2.0 * static_cast<double>(rand()) / RAND_MAX);
    };

    for (i = 0; i < m_size; i++)
    {
      for (j = 0; j < m_size; j++)
      {
        A[i][j] = rand_range();
      }
      b[i] = rand_range();
    }

    if (m_size <= 12)
    {
      printf("\n Matrix A (generated randomly):\n");
      for (i = 0; i < m_size; i++)
      {
        for (j = 0; j < m_size; j++)
        {
          printf("%9.6lf ", A[i][j]);
        }
        printf("\n");
      }

      printf("\n Vector b (generated randomly):\n");
      for (i = 0; i < m_size; i++)
      {
        printf("%9.6lf ", b[i]);
      }
      printf("\n\n");
    }
  }

  if (my_rank == 0)
  {
    time_begin = clock();
  }

  MPI_Bcast(&A[0][0], m_size * m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(b, m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (i = 0; i < m_size; i++)
  {
    map[i] = i % proc_count;
  }

  for (k = 0; k < m_size; k++)
  {
    MPI_Bcast(&A[k][k], m_size - k, MPI_DOUBLE, map[k], MPI_COMM_WORLD);
    MPI_Bcast(&b[k], 1, MPI_DOUBLE, map[k], MPI_COMM_WORLD);
    for (i = k + 1; i < m_size; i++)
    {
      if (map[i] == my_rank)
      {
        c[i] = A[i][k] / A[k][k];
      }
    }
    for (i = k + 1; i < m_size; i++)
    {
      if (map[i] == my_rank)
      {
        for (j = 0; j < m_size; j++)
        {
          A[i][j] = A[i][j] - (c[i] * A[k][j]);
        }
        b[i] = b[i] - (c[i] * b[k]);
      }
    }
  }

  if (my_rank == 0)
  {
    x[m_size - 1] = b[m_size - 1] / A[m_size - 1][m_size - 1];
    for (i = m_size - 2; i >= 0; i--)
    {
      sum = 0;
      for (j = i + 1; j < m_size; j++)
      {
        sum = sum + A[i][j] * x[j];
      }
      x[i] = (b[i] - sum) / A[i][i];
    }

    std::cout << "elapsed " << static_cast<double>(clock() - time_begin)
                 / CLOCKS_PER_SEC * 1000
              << " ms " << std::endl;

    if (m_size <= 12)
    {
      printf("\nThe solution is:");
      for (i = 0; i < m_size; i++)
      {
        printf("\nx%d=%f\t", i, x[i]);
      }
    }
  }

  MPI_Finalize();
  return 0;
}
