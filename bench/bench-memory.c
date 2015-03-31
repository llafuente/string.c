void bench_memory() {
  string* src = string_newc("fjksdfsofsijdflsdfisdflsdl");
  string_delete(&src);
}
