
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_TIME_UNITS_H
#define Z_INCLUDE_SYSCALLS_TIME_UNITS_H


#include <tracing/tracing_syscall.h>

#ifndef _ASMLANGUAGE

#include <syscall_list.h>
#include <syscall.h>

#include <linker/sections.h>


#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic push
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#if !defined(__XCC__)
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int z_impl_sys_clock_hw_cycles_per_sec_runtime_get(void);

__pinned_func
static inline int sys_clock_hw_cycles_per_sec_runtime_get(void)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke0(K_SYSCALL_SYS_CLOCK_HW_CYCLES_PER_SEC_RUNTIME_GET);
	}
#endif
	compiler_barrier();
	return z_impl_sys_clock_hw_cycles_per_sec_runtime_get();
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define sys_clock_hw_cycles_per_sec_runtime_get() ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYS_CLOCK_HW_CYCLES_PER_SEC_RUNTIME_GET, sys_clock_hw_cycles_per_sec_runtime_get); 	retval = sys_clock_hw_cycles_per_sec_runtime_get(); 	sys_port_trace_syscall_exit(K_SYSCALL_SYS_CLOCK_HW_CYCLES_PER_SEC_RUNTIME_GET, sys_clock_hw_cycles_per_sec_runtime_get, retval); 	retval; })
#endif
#endif


#ifdef __cplusplus
}
#endif

#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
#pragma GCC diagnostic pop
#endif

#endif
#endif /* include guard */
