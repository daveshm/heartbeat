static inline unsigned long rdtscp_start(void) {
  unsigned long var;
  unsigned int hi, lo;

  __asm volatile ("cpuid\n\t"
          "rdtsc\n\t" : "=a" (lo), "=d" (hi)
          :: "%rbx", "%rcx");

  var = ((unsigned long)hi << 32) | lo;
  return (var);
}

static inline unsigned long rdtscp_end(void) {
  unsigned long var;
  unsigned int hi, lo;

  __asm volatile ("rdtscp\n\t"
          "mov %%edx, %1\n\t"
          "mov %%eax, %0\n\t"
          "cpuid\n\t"  : "=r" (lo), "=r" (hi)
          :: "%rax", "%rbx", "%rcx", "%rdx");

  var = ((unsigned long)hi << 32) | lo;
  return (var);
  }

/*see https://www.intel.com/content/www/us/en/embedded/training/ia-32-ia-64-benchmark-code-execution-paper.html
 */
