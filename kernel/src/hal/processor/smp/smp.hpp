#ifndef SMP_HPP
#define SMP_HPP

#include <limine.h>
#include <mt/mt.hpp>

#define INVALID_CORE_ID -1
#define SMP_MAX_CORES 512

struct cpu_data_t
{
    int processor_id;
    unsigned int is_dispatched;
    tss_entry_t* tss;
};

namespace SMP
{
    extern limine_smp_request smp_request;
    extern cpu_data_t cpu_infos[SMP_MAX_CORES];
    void init_smp();
    void cpu_loop(limine_smp_info* info);
    cpu_data_t* get_cpu_data();
    void print_cpus();
    uint64_t get_core_count();
}

#endif