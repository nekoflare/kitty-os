#include <hal/processor/speed.hpp>
#include <hal/processor/access.hpp>

namespace Processor {
    double get_cpu_speed_mhz() {
        unsigned long long int start, end;

        // Measure time elapsed for a known number of cycles
        start = rdtsc();
        for (volatile int i = 0; i < 1000000; ++i) {} // Perform some time-consuming operation
        end = rdtsc();

        // Calculate CPU speed in MHz
        unsigned long long int cycles = end - start;
        double frequency = static_cast<double>(cycles) / 1000000.0; // Assuming each cycle corresponds to 1 MHz

        return frequency;
    }
}
