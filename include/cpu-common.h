#ifndef CPU_COMMON_H
#define CPU_COMMON_H 1

/* GETCPU interfaces that are target independent.  */

#ifndef CONFIG_USER_ONLY
#include "hwaddr.h"
#endif

#ifndef NEED_CPU_H
//#include "exec/poison.h"
#endif

#include "qemu/bswap.h"
#include "qemu/queue.h"

/**
 * CPUListState:
 * @cpu_fprintf: Print function.
 * @file: File to print to using @cpu_fprint.
 *
 * State commonly used for iterating over GETCPU models.
 */
typedef struct CPUListState {
    fprintf_function cpu_fprintf;
    FILE *file;
} CPUListState;

typedef enum MMUAccessType {
    MMU_DATA_LOAD  = 0,
    MMU_DATA_STORE = 1,
    MMU_INST_FETCH = 2
} MMUAccessType;

#if !defined(CONFIG_USER_ONLY)

enum device_endian {
    DEVICE_NATIVE_ENDIAN,
    DEVICE_BIG_ENDIAN,
    DEVICE_LITTLE_ENDIAN,
};

/* address in the RAM (different from a physical address) */
#if defined(CONFIG_XEN_BACKEND)
typedef uint64_t ram_addr_t;
#  define RAM_ADDR_MAX UINT64_MAX
#  define RAM_ADDR_FMT "%" PRIx64
#else
typedef uintptr_t ram_addr_t;
#  define RAM_ADDR_MAX UINTPTR_MAX
#  define RAM_ADDR_FMT "%" PRIxPTR
#endif

extern ram_addr_t ram_size;

/* memory API */

typedef void CPUWriteMemoryFunc(void *opaque, hwaddr addr, uint32_t value);
typedef uint32_t CPUReadMemoryFunc(void *opaque, hwaddr addr);

void vmx_ram_remap(ram_addr_t addr, ram_addr_t length);
/* This should not be used by devices.  */
VeertuMemArea *vmx_ram_addr_from_host(void *ptr, ram_addr_t *ram_addr);
void vmx_ram_set_idstr(ram_addr_t addr, const char *name, DeviceState *dev);
void vmx_ram_unset_idstr(ram_addr_t addr);

void cpu_physical_memory_rw(hwaddr addr, uint8_t *buf,
                            int len, int is_write);
static inline void cpu_physical_memory_read(hwaddr addr,
                                            void *buf, int len)
{
    cpu_physical_memory_rw(addr, buf, len, 0);
}
static inline void cpu_physical_memory_write(hwaddr addr,
                                             const void *buf, int len)
{
    cpu_physical_memory_rw(addr, (void *)buf, len, 1);
}
void *cpu_physical_memory_map(hwaddr addr,
                              hwaddr *plen,
                              int is_write);
void cpu_physical_memory_unmap(void *buffer, hwaddr len,
                               int is_write, hwaddr access_len);
void *cpu_register_map_client(void *opaque, void (*callback)(void *opaque));

bool cpu_physical_memory_is_io(hwaddr phys_addr);

/* Coalesced MMIO regions are areas where write operations can be reordered.
 * This usually implies that write operations are side-effect free.  This allows
 * batching which can make a major impact on performance when using
 * virtualization.
 */
void vmx_flush_coalesced_mmio_buffer(void);

uint32_t ldub_phys(VeertuAddressSpace *as, hwaddr addr);
uint32_t lduw_le_phys(VeertuAddressSpace *as, hwaddr addr);
uint32_t lduw_be_phys(VeertuAddressSpace *as, hwaddr addr);
uint32_t ldl_le_phys(VeertuAddressSpace *as, hwaddr addr);
uint32_t ldl_be_phys(VeertuAddressSpace *as, hwaddr addr);
uint64_t ldq_le_phys(VeertuAddressSpace *as, hwaddr addr);
uint64_t ldq_be_phys(VeertuAddressSpace *as, hwaddr addr);
void stb_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stw_le_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stw_be_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stl_le_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stl_be_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stq_le_phys(VeertuAddressSpace *as, hwaddr addr, uint64_t val);
void stq_be_phys(VeertuAddressSpace *as, hwaddr addr, uint64_t val);

//#ifdef NEED_CPU_H
uint32_t lduw_phys(VeertuAddressSpace *as, hwaddr addr);
uint32_t ldl_phys(VeertuAddressSpace *as, hwaddr addr);
uint64_t ldq_phys(VeertuAddressSpace *as, hwaddr addr);
void stl_phys_notdirty(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stw_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stl_phys(VeertuAddressSpace *as, hwaddr addr, uint32_t val);
void stq_phys(VeertuAddressSpace *as, hwaddr addr, uint64_t val);
//#endif

void cpu_physical_memory_write_rom(VeertuAddressSpace *as, hwaddr addr,
                                   const uint8_t *buf, int len);
void cpu_flush_icache_range(hwaddr start, int len);

extern struct VeertuMemArea io_mem_rom;
extern struct VeertuMemArea io_mem_notdirty;

typedef void (RAMBlockIterFunc)(void *host_addr,
    ram_addr_t offset, ram_addr_t length, void *opaque);

void vmx_ram_foreach_block(RAMBlockIterFunc func, void *opaque);

#endif

#endif /* !CPU_COMMON_H */
