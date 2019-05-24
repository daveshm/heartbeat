#include <time.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <sched.h>
#include "rdtscp.h"

using std::cout;  using std::endl;  using std::array;

#define CLOCK CLOCK_REALTIME

uint64_t to_ns(const timespec &ts);   // Converts a struct timespec to ns (since epoch).
void set_ticks_per_ns(bool set_rate); // Display or set tsc ticks per ns, _ticks_per_ns.
void get_start();             // Sets the 'start' time point: _start_tsc[in ticks] and _start_clock_time[in ns].
uint64_t tsc_to_ns(uint64_t tsc);     // Convert tsc ticks since _start_tsc to ns (since epoch) linearly using
                                      // _ticks_per_ns with origin(0) at the 'start' point set by get_start().

uint64_t _start_tsc, _start_clock_time; // The 'start' time point as both tsc tick number, start_tsc, and as
                                        // clock_gettime ns since epoch as _start_clock_time.
double _ticks_per_ns;                   // Calibrated in set_ticks_per_ns()

int main() {
	
  printf("cpu core: %d \n",sched_getcpu());
  set_ticks_per_ns(true); // Set _ticks_per_ns as the initial TSC ticks per ns.

  uint64_t tsc1, tsc2, tsc_now, tsc_ns, utc_ns;
  int64_t ns_diff;
  bool first_pass{true};
  for (;;) {
    timespec utc_now;
    if (first_pass) {
      get_start(); //Get start time in both ns since epoch (_start_clock_time), and tsc tick number(_start_tsc)
      cout << "_start_clock_time: " <<  _start_clock_time << ", _start_tsc: " << _start_tsc << endl;
      utc_ns = _start_clock_time;
      tsc_ns = tsc_to_ns(_start_tsc);   // == _start_clock_time by definition.
      tsc_now = _start_tsc;
      first_pass = false;
    } else {
      tsc1 = rdtscp_start();
      clock_gettime(CLOCK, &utc_now);
      tsc2 = rdtscp_end();
      tsc_now = (tsc1 + tsc2) / 2;
      tsc_ns = tsc_to_ns(tsc_now);
      utc_ns = to_ns(utc_now);
    }
    

    ns_diff = tsc_ns - (int64_t)utc_ns;

    cout << "elapsed ns: " << utc_ns - _start_clock_time << ", elapsed ticks: " << tsc_now - _start_tsc 
     << ", ns_diff: " << ns_diff << '\n' << endl;

	
    set_ticks_per_ns(false);  // Display current TSC ticks per ns (does not alter original _ticks_per_ns).
  }
}

void set_ticks_per_ns(bool set_rate) {
  constexpr int RUNS {1}, SLEEP{10};
  timespec clock_start, clock_end;
  uint64_t tsc1, tsc2, tsc_start, tsc_end;
  uint64_t elapsed_ns[RUNS], elapsed_ticks[RUNS];
  array<double, RUNS> rates; // ticks per ns from each run.

  if (set_rate) {
    clock_getres(CLOCK, &clock_start);
    cout <<  "Clock resolution: " << to_ns(clock_start) << "ns\n";
  }

  for (int i = 0; i < RUNS; ++i) {
    tsc1 = rdtscp_start();
    clock_gettime(CLOCK, &clock_start);
    tsc2 = rdtscp_end();                      
    tsc_start = (tsc1 + tsc2) / 2;

    usleep(1000);

    tsc1 = rdtscp_start();
    clock_gettime(CLOCK, &clock_end);
    tsc2 = rdtscp_end();                     
    tsc_end = (tsc1 + tsc2) / 2;

    elapsed_ticks[i] = tsc_end - tsc_start;
    elapsed_ns[i] = to_ns(clock_end) - to_ns(clock_start);
    rates[i] = static_cast<double>(elapsed_ticks[i]) / elapsed_ns[i];
  }

  cout << " tsc ticks      " << "ns     " << "tsc ticks per ns" << endl;
  for (int i = 0; i < RUNS; ++i)
    cout << elapsed_ticks[i] << " " << elapsed_ns[i] << " " << std::setprecision(12) << rates[i] << endl;
	
  sleep(1);

  if (set_rate)
    _ticks_per_ns = rates[RUNS-1];
}

constexpr uint64_t BILLION {1000000000};

uint64_t to_ns(const timespec &ts) {
  return ts.tv_sec * BILLION + ts.tv_nsec;
}

void get_start() { // Get start time both in tsc ticks as _start_tsc, and in ns since epoch as _start_clock_time
  timespec ts;
  uint64_t beg, end;

// loop to ensure we aren't interrupted between the two tsc reads
  while (1) {
    beg = rdtscp_start();
    clock_gettime(CLOCK, &ts);
    end = rdtscp_end();   
    if ((end - beg) <= 2000) // max ticks per clock call
      break;
  }

  _start_tsc = (end + beg) / 2;
  _start_clock_time = to_ns(ts); // converts timespec to ns since epoch
}

uint64_t tsc_to_ns(uint64_t tsc) { // Convert tsc ticks into absolute ns:
  // Absolute ns is defined by this linear extrapolation from the start point where
  //_start_tsc[in ticks] corresponds to _start_clock_time[in ns].
  uint64_t diff = tsc - _start_tsc;
  return _start_clock_time + static_cast<uint64_t>(diff / _ticks_per_ns);
}
