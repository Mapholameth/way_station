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

static const unsigned m_size = 8;

auto measure = [](std::function<void()> f)
{
  const int totalAttempts = 10;
  double minTime = std::numeric_limits<double>::max();
  for (int tries = 0; tries < totalAttempts; tries++)
  {
    std::cout << tries << "..";
    clock_t begin = clock();
    f();
    clock_t end = clock();
    minTime = std::min(double(end - begin) / CLOCKS_PER_SEC, minTime);
  }
  std::cout << "elapsed " << minTime * 1000 << " ms " << std::endl;
};

void fill_matrix(double m[m_size][m_size])
{
  static int n = 0;
  for (unsigned i = 0; i < m_size; i++)
  {
    for (unsigned j = 0; j < m_size; j++)
    {
      m[i][j] = n++;
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
      printf("%5.0f ", m[i][j]);
    }
    printf("|");
  }
}

int main(int argc, char *argv[])
{
  double A[m_size][m_size];
  double B[m_size][m_size];
  double C[m_size][m_size];
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
    fill_matrix(A);
    fill_matrix(B);
  }

  // TODO: use scattterv, gatherv
  MPI_Bcast(B, m_size * m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(A, m_size * m_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
//  MPI_Scatter(A, m_size * m_size / P, MPI_INT,
//              A[from], m_size * m_size / P, MPI_INT, 0, MPI_COMM_WORLD);

  double pes[m_size];
  printf("computing slice %d (from row %d to %d)\n", my_rank, from, to - 1);
  for (i = from; i < to; i++)
  {
    for (j = 0; j < m_size; j++)
    {
      pes[j] = 0.0;
      for (k = 0; k < m_size; k++)
      {
        pes[j] += A[i][k] * B[k][j];
      }
    }
  }

  measure([&](){
    MPI_Gather(pes, m_size * m_size /proc_count, MPI_DOUBLE,
               C, m_size * m_size / proc_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  });

  if (my_rank == 0)
  {
    printf("\n\n");
    print_matrix(A);
    printf("\n\n\t       * \n");
    print_matrix(B);
    printf("\n\n\t       = \n");
    print_matrix(C);
    printf("\n\n");
  }

  MPI_Finalize();
  return 0;
}
