// ministat

#include <time.h>
#include <stdlib.h>

struct timespec *bench_time_start,
  *bench_time_end;

char* bench_get_current_date();

void bench_start();

double bench_end();


// create a benchmark
// allways run times+3
// the first three times are to get everything hot
#define BENCHMARK(prefix, measures, times, func) \
{ \
  func(); bench_start(); bench_end();\
  func(); bench_start(); bench_end();\
  func(); bench_start(); bench_end();\
  double lmeasures[measures]; \
  for (size_t m = 0; m < measures; ++m) { \
    bench_start(); \
    for (size_t i = 0; i < times; ++i) { \
      func(); \
    } \
    lmeasures[m] = bench_end(); \
  } \
  char filename[100]; \
  sprintf(filename, "bench_data/%s-%s", prefix, #func); \
  FILE* fp = fopen (filename , "w+"); \
  char* date = bench_get_current_date(); \
  fprintf(fp, "# " #func ": %s\n", date); \
  free(date); \
  for (size_t m = 0; m < measures; ++m) { \
    fprintf(fp, "%f\n", lmeasures[m]); \
  } \
  fclose(fp); \
}

#include "bench.c"
