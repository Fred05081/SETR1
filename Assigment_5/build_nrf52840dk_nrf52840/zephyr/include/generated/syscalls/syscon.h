
/* auto-generated by gen_syscalls.py, don't edit */
#ifndef Z_INCLUDE_SYSCALLS_SYSCON_H
#define Z_INCLUDE_SYSCALLS_SYSCON_H


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

extern int z_impl_syscon_get_base(const struct device * dev, uintptr_t * addr);

__pinned_func
static inline int syscon_get_base(const struct device * dev, uintptr_t * addr)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&addr, K_SYSCALL_SYSCON_GET_BASE);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_get_base(dev, addr);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_get_base(dev, addr) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_GET_BASE, syscon_get_base, dev, addr); 	retval = syscon_get_base(dev, addr); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_GET_BASE, syscon_get_base, dev, addr, retval); 	retval; })
#endif
#endif


extern int z_impl_syscon_read_reg(const struct device * dev, uint16_t reg, uint32_t * val);

__pinned_func
static inline int syscon_read_reg(const struct device * dev, uint16_t reg, uint32_t * val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke3(*(uintptr_t *)&dev, *(uintptr_t *)&reg, *(uintptr_t *)&val, K_SYSCALL_SYSCON_READ_REG);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_read_reg(dev, reg, val);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_read_reg(dev, reg, val) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_READ_REG, syscon_read_reg, dev, reg, val); 	retval = syscon_read_reg(dev, reg, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_READ_REG, syscon_read_reg, dev, reg, val, retval); 	retval; })
#endif
#endif


extern int z_impl_syscon_write_reg(const struct device * dev, uint16_t reg, uint32_t val);

__pinned_func
static inline int syscon_write_reg(const struct device * dev, uint16_t reg, uint32_t val)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke3(*(uintptr_t *)&dev, *(uintptr_t *)&reg, *(uintptr_t *)&val, K_SYSCALL_SYSCON_WRITE_REG);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_write_reg(dev, reg, val);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_write_reg(dev, reg, val) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_WRITE_REG, syscon_write_reg, dev, reg, val); 	retval = syscon_write_reg(dev, reg, val); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_WRITE_REG, syscon_write_reg, dev, reg, val, retval); 	retval; })
#endif
#endif


extern int z_impl_syscon_get_size(const struct device * dev, size_t * size);

__pinned_func
static inline int syscon_get_size(const struct device * dev, size_t * size)
{
#ifdef CONFIG_USERSPACE
	if (z_syscall_trap()) {
		/* coverity[OVERRUN] */
		return (int) arch_syscall_invoke2(*(uintptr_t *)&dev, *(uintptr_t *)&size, K_SYSCALL_SYSCON_GET_SIZE);
	}
#endif
	compiler_barrier();
	return z_impl_syscon_get_size(dev, size);
}

#if (CONFIG_TRACING_SYSCALL == 1)
#ifndef DISABLE_SYSCALL_TRACING

#define syscon_get_size(dev, size) ({ 	int retval; 	sys_port_trace_syscall_enter(K_SYSCALL_SYSCON_GET_SIZE, syscon_get_size, dev, size); 	retval = syscon_get_size(dev, size); 	sys_port_trace_syscall_exit(K_SYSCALL_SYSCON_GET_SIZE, syscon_get_size, dev, size, retval); 	retval; })
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
