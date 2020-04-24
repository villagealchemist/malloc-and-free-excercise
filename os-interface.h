#ifndef os_interface_h_
#define os_interface_h_

#include <stddef.h>

#if defined(__cplusplus)
extern "C" {
#endif

void *os_allocate_memory(size_t *size);
void os_return_memory(void *ptr, size_t size);

#if defined(__cplusplus)
}
#endif
#endif
