char* bench_get_current_date() {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  char* out = (char*) malloc(sizeof(char) * 50);

  sprintf(out, "%d-%d-%d %d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
  return out;
}

void bench_start() {
  bench_time_start = (struct timespec*) malloc(sizeof(bench_time_start));
  //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, bench_time_start);
  clock_gettime(CLOCK_MONOTONIC, bench_time_start);
}

double bench_end() {
  bench_time_end = (struct timespec*) malloc(sizeof(bench_time_end));
  //clock_gettime(CLOCK_PROCESS_CPUTIME_ID, bench_time_end);
  clock_gettime(CLOCK_MONOTONIC, bench_time_end);

  double result = (
  bench_time_end->tv_sec - bench_time_start->tv_sec
  ) * 1E9 + (
  bench_time_end->tv_nsec - bench_time_start->tv_nsec
  );
  /*
  if ((bench_time_end->tv_nsec - bench_time_start->tv_nsec)<0) {
  result = ( - bench_time_start->tv_sec - 1) * 1000000000;
  result += 1000000000 - bench_time_end->tv_nsec - bench_time_start->tv_nsec;
} else {
result = (bench_time_end->tv_sec - bench_time_start->tv_sec) * 1000000000;
result += bench_time_end->tv_nsec - bench_time_start->tv_nsec;
}
*/

free(bench_time_start);
bench_time_start = 0;
free(bench_time_end);
bench_time_end = 0;
return result;
}
