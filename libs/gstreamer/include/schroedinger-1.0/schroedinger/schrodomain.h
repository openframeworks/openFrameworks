
#ifndef _SCHRO_DOMAIN_H_
#define _SCHRO_DOMAIN_H_

#include <schroedinger/schroasync.h>

typedef struct _SchroMemoryDomain SchroMemoryDomain;

#ifdef SCHRO_ENABLE_UNSTABLE_API

#define SCHRO_MEMORY_DOMAIN_SLOTS 1000

struct _SchroMemoryDomain {
  SchroMutex * mutex;

  unsigned int flags;

  void *(*alloc) (int size);
  void *(*alloc_2d) (int depth, int width, int height);
  void (*free) (void *ptr, int size);

  struct {
    unsigned int flags;
    void *ptr;
    int size;
    void *priv;
  } slots[SCHRO_MEMORY_DOMAIN_SLOTS];
};

#define SCHRO_EXEC_DOMAIN_CPU 0x0001
#define SCHRO_EXEC_DOMAIN_CUDA 0x0002
#define SCHRO_EXEC_DOMAIN_OPENGL 0x0003

#define SCHRO_MEMORY_DOMAIN_CPU 0x0001
#define SCHRO_MEMORY_DOMAIN_CUDA 0x0002
#define SCHRO_MEMORY_DOMAIN_OPENGL 0x0004

#define SCHRO_MEMORY_DOMAIN_SLOT_ALLOCATED 0x0001
#define SCHRO_MEMORY_DOMAIN_SLOT_IN_USE 0x0002


SchroMemoryDomain * schro_memory_domain_new (void);
SchroMemoryDomain * schro_memory_domain_new_local (void);
void schro_memory_domain_free (SchroMemoryDomain *domain);

void * schro_memory_domain_alloc (SchroMemoryDomain *domain, int size);
void * schro_memory_domain_alloc_2d (SchroMemoryDomain *domain,
    int depth, int width, int height);
void schro_memory_domain_memfree (SchroMemoryDomain *domain, void *ptr);

#endif

#endif

