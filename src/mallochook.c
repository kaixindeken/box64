#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/mman.h>
#include <dlfcn.h>

#include "box64context.h"
#include "debug.h"
#include "callback.h"
#include "librarian.h"
#include "elfs/elfloader_private.h"
#include "custommem.h"
#include "symbols.h"

/*
    This file here is for handling overriding of malloc functions

 Libraries like tcmalloc overrides all malloc/free/new/delete function and implement a custom version.
 Problem is, box64 is already loaded in memoery, and already using system libc malloc for many of it's things
 before those lib are even loaded in memory.
 Also, those override function can be define in the main executable, or in a lib loaded directly by the exectable
 or even in a lib loaded later using dlsym.

 The 2 different strategies to handle this can be defined as "Embrace" and "Exterminate" (as it cannot simply be ignored, 
 or you end up with mixing free/realloc from one lib and malloc/free from libc)

 In the "Embrace" strategy, the overridden malloc function are taken into account as soon as possible, and are used for all loaded lib, including native
 In the "Exterminate" strategy, the overridden malloc function are erased, and replaced with libc malloc as soon as they are defined.

 The advantage of "Embrace" is that the system will run the function it supposed to be using, and potential side-effect and staticaly linked functions 
 will run as intended.
 The downside of "Embrace" is that is makes it impossible to load a library with dlsym that override malloc function, especialy 
 if it loads natively wrapped function

 The advantage of "Exterminate" is that you wont be emulating basic malloc and friend, wich are used extensively in every program. Also, loading lib 
 with dlopen will not a be a problem.
 The downside of "Exterminate" is that side effect are less well controled. Staticaly linked stuff and anonymous symbols might put this strategy in trouble.

 This is the Exterminate strategy implementation
*/

#include "bridge.h"
#include "tools/bridge_private.h"
#include "wrapper.h"

#define SUPER()                 \
GO(malloc, pFL)                 \
GO(free, vFp)                   \
GO(calloc, pFLL)                \
GO(realloc, pFpL)               \
GO(aligned_alloc, pFLL)         \
GO(memalign, pFLL)              \
GO(posix_memalign, iFpLL)       \
GO(pvalloc, pFL)                \
GO(valloc, pFL)                 \
GO(cfree, vFp)                  \
GO(malloc_usable_size, LFp)     \
GO2(_Znwm, pFL)                 \
GO2(_ZnwmRKSt9nothrow_t, pFLp)  \
GO2(_Znam, pFL)                 \
GO2(_ZnamRKSt9nothrow_t, pFLp)  \
GO2(_ZdaPv, vFp)                \
GO2(_ZdaPvm, vFpL)              \
GO2(_ZdaPvmSt11align_val_t, vFpLL)              \
GO2(_ZdlPv, vFp)                                \
GO2(_ZdlPvm, vFpL)                              \
GO2(_ZnwmSt11align_val_t, pFLL)                 \
GO2(_ZnwmSt11align_val_tRKSt9nothrow_t, pFLLp)  \
GO2(_ZnamSt11align_val_t, pFLL)                 \
GO2(_ZnamSt11align_val_tRKSt9nothrow_t, pFLLp)  \
GO2(_ZdlPvRKSt9nothrow_t, vFpp)                 \
GO2(_ZdaPvSt11align_val_tRKSt9nothrow_t, vFpLp) \
GO2(_ZdlPvmSt11align_val_t, vFpLL)              \
GO2(_ZdaPvRKSt9nothrow_t, vFpp)                 \
GO2(_ZdaPvSt11align_val_t, vFpL)                \
GO2(_ZdlPvSt11align_val_t, vFpL)                \
GO2(_ZdlPvSt11align_val_tRKSt9nothrow_t, vFpLp) \
GO2(tc_calloc, pFLL)            \
GO2(tc_cfree, vFp)              \
GO2(tc_delete, vFp)             \
GO2(tc_deletearray, vFp)        \
GO2(tc_deletearray_nothrow, vFpp)               \
GO2(tc_delete_nothrow, vFpp)    \
GO2(tc_free, vFp)               \
GO2(tc_malloc, pFL)             \
GO2(tc_malloc_size, LFp)        \
GO2(tc_new, pFL)                \
GO2(tc_new_nothrow, pFLp)       \
GO2(tc_newarray, pFL)           \
GO2(tc_newarray_nothrow, pFLp)  \
GO2(tc_pvalloc, pFL)            \
GO2(tc_valloc, pFL)             \
GO2(tc_memalign, pFLL)          \
GO2(tc_malloc_skip_new_handler_weak, pFL)       \
GO2(tc_mallocopt, iFii)         \
GO2(tc_malloc_stats, vFv)       \
GO2(tc_malloc_skip_new_handler, pFL)            \
GO2(tc_mallinfo, pFp)           \
GO2(tc_posix_memalign, iFpLL)   \
GO2(tc_realloc, pFpL)           \
GO2(safer_scalable_aligned_realloc, pFpLLp)     \
GO2(safer_scalable_free, vFpp)  \
GO2(safer_scalable_msize, LFpp) \
GO2(safer_scalable_realloc, pFpLp)              \
GO2(scalable_aligned_free, vFp)                 \
GO2(scalable_aligned_malloc, pFLL)              \
GO2(scalable_msize, LFp)        \


//GO2(tc_set_new_mode, iFi) 
//GO2(tc_version, iFi) 

typedef void  (vFv_t)   (void);
typedef int   (iFv_t)   (void);
typedef int   (iFi_t)   (int);
typedef void* (*pFL_t)  (size_t);
typedef void* (*pFLp_t) (size_t, void* p);
typedef void  (*vFp_t)  (void*);
typedef void* (*pFp_t)  (void*);
typedef size_t(*LFp_t)  (void*);
typedef int   (*iFii_t) (int, int);
typedef void  (*vFpp_t) (void*, void*);
typedef size_t(*LFpp_t) (void*, void*);
typedef void  (*vFpL_t) (void*, size_t);
typedef void* (*pFLL_t) (size_t, size_t);
typedef void* (*pFLLp_t)(size_t, size_t, void* p);
typedef void  (*vFpLp_t)(void*, size_t, void*);
typedef void* (*pFpLp_t)(void*, size_t, void*);
typedef void  (*vFpLL_t)(void*, size_t, size_t);
typedef void* (*pFpLLp_t)(void*, size_t, size_t, void*);

#ifdef ANDROID
void*(*__libc_malloc)(size_t) = NULL;
void*(*__libc_realloc)(size_t, void*) = NULL;
void*(*__libc_calloc)(size_t, size_t) = NULL;
void (*__libc_free)(void*) = NULL;
void*(*__libc_memalign)(size_t, size_t) = NULL;
#endif
size_t(*box_malloc_usable_size)(void*) = NULL;

int GetTID();
uint32_t getProtection(uintptr_t addr);
// malloc_hack "2" handling
// mmap history
static int malloc_hack_2 = 0;

char* box_strdup(const char* s) {
    char* ret = box_calloc(1, strlen(s)+1);
    memcpy(ret, s, strlen(s));
    return ret;
}

char* box_realpath(const char* path, char* ret)
{
    if(ret)
        return realpath(path, ret);
#ifdef PATH_MAX
    size_t path_max = PATH_MAX;
#else
    size_t path_max = pathconf(path, _PC_PATH_MAX);
    if (path_max <= 0)
    path_max = 4096;
#endif
    char tmp[path_max];
    char* p = realpath(path, tmp);
    if(!p)
        return NULL;
    return box_strdup(tmp);
}

static size_t pot(size_t l) {
    size_t ret = 0;
    while (l>(1<<ret))  ++ret;
    return 1<<ret;
}

static int ispot(size_t l) {
    return pot(l)==l;
}

// redefining all libc memory allocation routines
EXPORT void* malloc(size_t l)
{
    return box_calloc(1, l);
}

static uintptr_t real_free = 0;
EXPORT void free(void* p)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void* calloc(size_t n, size_t s)
{
    return box_calloc(n, s);
}

EXPORT void* realloc(void* p, size_t s)
{
    if(malloc_hack_2)
        if(getMmapped((uintptr_t)p)) {
            // found! Will realloc using regular malloc then copy from old address as much as possible, but need to check size first
            void* ret = box_malloc(s);
            printf_log(LOG_DEBUG, "Malloc_Hack_2: hacking realloc(%p, %zu)", p, s);
            while(s && !(getProtection((uintptr_t)p+s)&PROT_READ)) {if(s>box64_pagesize) s-=box64_pagesize; else s=0;}
            memcpy(ret, p, s);
            printf_log(LOG_DEBUG, " -> %p (copied %zu from old)\n", ret, s);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return ret;
        }
    return box_realloc(p, s);
}

EXPORT void* aligned_alloc(size_t align, size_t size)
{
    return box_memalign(align, size);
}

EXPORT void* memalign(size_t align, size_t size)
{
    return box_memalign(align, size);
}

EXPORT int posix_memalign(void** p, size_t align, size_t size)
{
    if(align%sizeof(void*) || pot(align)!=align)
        return EINVAL;
    void* ret = box_memalign(align, size);
    if(!ret)
        return ENOMEM;
    *p = ret;
    return 0;
}

EXPORT void* valloc(size_t size)
{
    return box_memalign(box64_pagesize, size);
}

EXPORT void* pvalloc(size_t size)
{
    return box_memalign(box64_pagesize, (size+box64_pagesize-1)&~(box64_pagesize-1));
}

EXPORT void cfree(void* p)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT size_t malloc_usable_size(void* p)
{
    return box_malloc_usable_size(p);
}

EXPORT void* my__Znwm(size_t sz)   //operator new(size_t)
{
    return box_malloc(sz);
}

EXPORT void* my__ZnwmRKSt9nothrow_t(size_t sz, void* p)   //operator new(size_t, std::nothrow_t const&)
{
    return box_malloc(sz);
}

EXPORT void* my__Znam(size_t sz)   //operator new[](size_t)
{
    return box_malloc(sz);
}

EXPORT void* my__ZnamRKSt9nothrow_t(size_t sz, void* p)   //operator new[](size_t, std::nothrow_t const&)
{
    return box_malloc(sz);
}


EXPORT void my__ZdaPv(void* p)   //operator delete[](void*)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdaPvm(void* p, size_t sz)   //operator delete[](void*, size_t)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdaPvmSt11align_val_t(void* p, size_t sz, size_t align)   //operator delete[](void*, unsigned long, std::align_val_t)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdlPv(void* p)   //operator delete(void*)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdlPvm(void* p, size_t sz)   //operator delete(void*, size_t)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void* my__ZnwmSt11align_val_t(size_t sz, size_t align)  //// operator new(unsigned long, std::align_val_t)
{
    return box_memalign(align, sz);
}

EXPORT void* my__ZnwmSt11align_val_tRKSt9nothrow_t(size_t sz, size_t align, void* p)  //// operator new(unsigned long, std::align_val_t, std::nothrow_t const&)
{
    return box_memalign(align, sz);
}

EXPORT void* my__ZnamSt11align_val_t(size_t sz, size_t align)  //// operator new[](unsigned long, std::align_val_t)
{
    return box_memalign(align, sz);
}

EXPORT void* my__ZnamSt11align_val_tRKSt9nothrow_t(size_t sz, size_t align, void* p)  //// operator new[](unsigned long, std::align_val_t, std::nothrow_t const&)
{
    return box_memalign(align, sz);
}

EXPORT void my__ZdlPvRKSt9nothrow_t(void* p, void* n)   //operator delete(void*, std::nothrow_t const&)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdaPvSt11align_val_tRKSt9nothrow_t(void* p, size_t align, void* n)   //operator delete[](void*, std::align_val_t, std::nothrow_t const&)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdlPvmSt11align_val_t(void* p, size_t sz, size_t align)   //operator delete(void*, unsigned long, std::align_val_t)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdaPvRKSt9nothrow_t(void* p, void* n)   //operator delete[](void*, std::nothrow_t const&)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdaPvSt11align_val_t(void* p, size_t align)   //operator delete[](void*, std::align_val_t)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdlPvSt11align_val_t(void* p, size_t align)   //operator delete(void*, std::align_val_t)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void my__ZdlPvSt11align_val_tRKSt9nothrow_t(void* p, size_t align, void* n)   //operator delete(void*, std::align_val_t, std::nothrow_t const&)
{
    if(malloc_hack_2 && p) {
        if(getMmapped((uintptr_t)p)) {
            printf_log(LOG_DEBUG, "%04d|Malloc_Hack_2: not freeing %p\n", GetTID(), p);
            // Mmaped, free with original function
            if(real_free)
                RunFunctionFmt(my_context, real_free, "vFp", p);
            return;
        }
    }
    box_free(p);
}

EXPORT void* my_tc_calloc(size_t n, size_t s)
{
    return box_calloc(n, s);
}

EXPORT void my_tc_cfree(void* p)
{
    box_free(p);
}

EXPORT void my_tc_delete(void* p)
{
    box_free(p);
}

EXPORT void my_tc_deletearray(void* p)
{
    box_free(p);
}

EXPORT void my_tc_deletearray_nothrow(void* p, void* n)
{
    box_free(p);
}

EXPORT void my_tc_delete_nothrow(void* p, void* n)
{
    box_free(p);
}

EXPORT void my_tc_free(void* p)
{
    box_free(p);
}

EXPORT void* my_tc_malloc(size_t s)
{
    return box_calloc(1, s);
}

EXPORT size_t my_tc_malloc_size(void* p)
{
    return box_malloc_usable_size(p);
}

EXPORT void* my_tc_new(size_t s)
{
    return box_calloc(1, s);
}

EXPORT void* my_tc_new_nothrow(size_t s, void* n)
{
        return box_calloc(1, s);
}

EXPORT void* my_tc_newarray(size_t s)
{
        return box_calloc(1, s);
}

EXPORT void* my_tc_newarray_nothrow(size_t s, void* n)
{
        return box_calloc(1, s);
}

EXPORT void* my_tc_pvalloc(size_t size)
{
    return box_memalign(box64_pagesize, (size+box64_pagesize-1)&~(box64_pagesize-1));
}

EXPORT void* my_tc_valloc(size_t size)
{
    return box_memalign(box64_pagesize, size);
}

EXPORT void* my_tc_memalign(size_t align, size_t size)
{
    return box_memalign(align, size);
}

EXPORT void* my_tc_malloc_skip_new_handler_weak(size_t s)
{
    return box_calloc(1, s);
}

EXPORT int my_tc_mallocopt(int param, int value)
{
    // ignoring...
    return 1;
}

EXPORT void my_tc_malloc_stats()
{
    // ignoring
}
/*
EXPORT int my_tc_set_new_mode(int mode)
{
    // ignoring
    static int old = 0;
    int ret = old;
    old = mode;
    return ret;
}
*/
EXPORT void* my_tc_malloc_skip_new_handler(size_t s)
{
    return box_calloc(1, s);
}

EXPORT void* my_tc_mallinfo(void* p)
{
    // ignored, returning null stuffs
    memset(p, 0, sizeof(size_t)*10);
    return p;
}

EXPORT int my_tc_posix_memalign(void** p, size_t align, size_t size)
{
    if(align%sizeof(void*) || pot(align)!=align)
        return EINVAL;
    void* ret = box_memalign(align, size);
    if(!ret)
        return ENOMEM;
    *p = ret;
    return 0;
}

EXPORT void* my_tc_realloc(void* p, size_t s)
{
    return box_realloc(p, s);
}
/*
EXPORT int my_tc_version(int i)
{
    return 2;
}
*/

EXPORT void* my_safer_scalable_aligned_realloc(void* p, size_t size, size_t align, void *old)
{
    if(!ispot(align)) {
        errno = EINVAL;
        return NULL;
    }
    if(align <= 8)
        return box_realloc(p, size);
    size_t old_size = box_malloc_usable_size(p);
    if(old_size>=size)
        return p;
    void* new_p = box_memalign(align, size);
    memcpy(new_p, p, (old_size<size)?old_size:size);
    box_free(p);
    return p;
}

EXPORT void my_safer_scalable_free(void*p , void* old)
{
    box_free(p);
}

EXPORT size_t my_safer_scalable_msize(void* p, void* old)
{
    return box_malloc_usable_size(p);
}

EXPORT void* my_safer_scalable_realloc(void* p, size_t size, void* old)
{
    return box_realloc(p, size);
}

EXPORT void my_scalable_aligned_free(void* p)
{
    box_free(p);
}

EXPORT void* my_scalable_aligned_malloc(size_t size, size_t align)
{
    if(!ispot(align)) {
        errno = EINVAL;
        return NULL;
    }
    if(align <= 8)
        return box_malloc(size);
    return box_memalign(align, size);
}

EXPORT void* my_scalable_aligned_realloc(void* p, size_t size, size_t align)
{
    if(!ispot(align)) {
        errno = EINVAL;
        return NULL;
    }
    if(align <= 8)
        return box_realloc(p, size);
    size_t old_size = box_malloc_usable_size(p);
    if(old_size>=size)
        return p;
    void* new_p = box_memalign(align, size);
    memcpy(new_p, p, (old_size<size)?old_size:size);
    box_free(p);
    return p;
}

EXPORT size_t my_scalable_msize(void* p)
{
    return box_malloc_usable_size(p);
}


#pragma pack(push, 1)
typedef struct reloc_jmp_s {
    uint8_t _ff;
    uint8_t _25;
    uint32_t _00;
    void* addr;
} reloc_jmp_t;
typedef struct simple_jmp_s {
    uint8_t _e9;
    uint32_t delta;
} simple_jmp_t;
#pragma pack(pop)

static void addRelocJmp(void* offs, void* where, size_t size, const char* name, elfheader_t* h)
{
    if(0 && malloc_hack_2 && !strcmp(name, "free")) {
        if(!real_free /*&& !strcmp(name, "free")*/)
            real_free = (uintptr_t)offs;
        // a bridge jump is roughly 32 bytes
        ForceUpdateSymbol(h->mapsymbols, name, (uintptr_t)where, 32);
        ForceUpdateSymbol(h->weaksymbols, name, (uintptr_t)where, 32);
        ForceUpdateSymbol(h->localsymbols, name, (uintptr_t)where, 32);
    } else {
        reloc_jmp_t r_jmp = {0};
        simple_jmp_t s_jmp = {0};
        size_t sz = 0;
        intptr_t off64 = (intptr_t)where - ((intptr_t)offs+5);
        void* p = NULL;
        int32_t off32 = (int32_t)off64;
        if(off32 == off64) {
            s_jmp._e9 = 0xe9;
            s_jmp.delta = (uint32_t)off32;
            p = &s_jmp;
            sz = sizeof(s_jmp);
        } else {
            r_jmp._ff = 0xff;
            r_jmp._25 = 0x25;
            r_jmp.addr = where;
            p = &r_jmp;
            sz = sizeof(r_jmp);
        }
        if(size>=sz)
            memcpy(offs, p, sz);
        else {
            printf_log(LOG_INFO, "Warning, cannot redirect %s, too small %zu vs %zu\n", name, size, sz);
            // use plan-B, it might be enough
            ForceUpdateSymbol(h->mapsymbols, name, (uintptr_t)where, 32);
            ForceUpdateSymbol(h->weaksymbols, name, (uintptr_t)where, 32);
            ForceUpdateSymbol(h->localsymbols, name, (uintptr_t)where, 32);
        }
    }
}

void checkHookedSymbols(elfheader_t* h)
{
    int hooked = 0;
    if(box64_malloc_hack==1)
        return;
    for (size_t i=0; i<h->numDynSym && hooked<2; ++i) {
        const char * symname = h->DynStr+h->DynSym[i].st_name;
        int bind = ELF64_ST_BIND(h->DynSym[i].st_info);
        int type = ELF64_ST_TYPE(h->DynSym[i].st_info);
        int vis = h->DynSym[i].st_other&0x3;
        if((type==STT_FUNC) 
        && (vis==STV_DEFAULT || vis==STV_PROTECTED) && (h->DynSym[i].st_shndx!=0 && h->DynSym[i].st_shndx<=65521)) {
            uintptr_t offs = h->DynSym[i].st_value + h->delta;
            size_t sz = h->DynSym[i].st_size;
            if(bind!=STB_LOCAL && bind!=STB_WEAK && sz>=sizeof(reloc_jmp_t)) {
                #define GO(A, B) if(!strcmp(symname, #A)) ++hooked; else if(!strcmp(symname, "scalable_" #A)) ++hooked; else if(!strcmp(symname, "__TBB_internal_" #A)) ++hooked;
                #define GO2(A, B)
                SUPER()
                #undef GO
                #undef GO2
            }
        }
    }
    if(hooked<2)
        return; // only redirect on lib that hooked / redefined the operators
    if(box64_malloc_hack==2)
        malloc_hack_2 = 1;
    printf_log(LOG_INFO, "Redirecting overridden malloc%s function for %s\n", malloc_hack_2?" with hack":"", ElfName(h));
    for (size_t i=0; i<h->numDynSym; ++i) {
        const char * symname = h->DynStr+h->DynSym[i].st_name;
        int bind = ELF64_ST_BIND(h->DynSym[i].st_info);
        int type = ELF64_ST_TYPE(h->DynSym[i].st_info);
        int vis = h->DynSym[i].st_other&0x3;
        if((type==STT_FUNC) 
        && (vis==STV_DEFAULT || vis==STV_PROTECTED) && (h->DynSym[i].st_shndx!=0 && h->DynSym[i].st_shndx<=65521)) {
            uintptr_t offs = h->DynSym[i].st_value + h->delta;
            size_t sz = h->DynSym[i].st_size;
            if(bind!=STB_LOCAL && bind!=STB_WEAK) {
                #define GO(A, B) if(!strcmp(symname, "__libc_" #A)) {uintptr_t alt = AddCheckBridge(my_context->system, B, A, 0, #A); printf_log(LOG_DEBUG, "Redirecting %s function from %p (%s)\n", symname, (void*)offs, ElfName(h)); addRelocJmp((void*)offs, (void*)alt, sz, #A, h);}
                #define GO2(A, B)
                SUPER()
                #undef GO
                #undef GO2
                #define GO(A, B) if(!strcmp(symname, "scalable_" #A)) {uintptr_t alt = AddCheckBridge(my_context->system, B, A, 0, #A); printf_log(LOG_DEBUG, "Redirecting %s function from %p (%s)\n", symname, (void*)offs, ElfName(h)); addRelocJmp((void*)offs, (void*)alt, sz, #A, h);}
                #define GO2(A, B)
                SUPER()
                #undef GO
                #undef GO2
                #define GO(A, B) if(!strcmp(symname, "__TBB_internal_" #A)) {uintptr_t alt = AddCheckBridge(my_context->system, B, A, 0, #A); printf_log(LOG_DEBUG, "Redirecting %s function from %p (%s)\n", symname, (void*)offs, ElfName(h)); addRelocJmp((void*)offs, (void*)alt, sz, #A, h);}
                #define GO2(A, B)
                SUPER()
                #undef GO
                #undef GO2
                #define GO(A, B) if(!strcmp(symname, #A)) {uintptr_t alt = AddCheckBridge(my_context->system, B, A, 0, #A); printf_log(LOG_DEBUG, "Redirecting %s function from %p (%s)\n", symname, (void*)offs, ElfName(h)); addRelocJmp((void*)offs, (void*)alt, sz, #A, h);}
                #define GO2(A, B) if(!strcmp(symname, #A)) {uintptr_t alt = AddCheckBridge(my_context->system, B, my_##A, 0, #A); printf_log(LOG_DEBUG, "Redirecting %s function from %p (%s)\n", symname, (void*)offs, ElfName(h)); addRelocJmp((void*)offs, (void*)alt, sz, #A, h);}
                SUPER()
                #undef GO
                #undef GO2
            }
        }
    }
}

EXPORT int my___TBB_internal_find_original_malloc(int n, char* names[], void* ptr[])
{
    int ret = 1;
    #define GO(A, B) else if(!strcmp(names[i], #A)) {ptr[i] = A;}
    #define GO2(A, B) 
    for (int i=0; i<n; ++i)
        if (0) {}
        SUPER()
        else ret = 0;
    return ret;
    #undef GO
    #undef GO2
}

EXPORT void my___TBB_call_with_my_server_info(void* cb, void* server)
{
    // nothing
}

EXPORT int my___TBB_make_rml_server(void* factory, void* server, void* client)
{
    // nothing
    return 0;
}

EXPORT void my___RML_close_factory(void* server)
{
    // nothing
}

EXPORT int my___RML_open_factory(void* factory, void* server_version, int client_version)
{
    // nothing
    return 0;
}

void init_malloc_hook() {
#ifdef ANDROID
    __libc_malloc = dlsym(RTLD_NEXT, "malloc");
    __libc_realloc = dlsym(RTLD_NEXT, "realloc");
    __libc_calloc = dlsym(RTLD_NEXT, "calloc");
    __libc_free = dlsym(RTLD_NEXT, "free");
    __libc_memalign = dlsym(RTLD_NEXT, "memalign");
#endif
    box_malloc_usable_size = dlsym(RTLD_NEXT, "malloc_usable_size");
    #if 0
    #define GO(A, B)
    #define GO2(A, B)   box_##A = (B##_t)dlsym(RTLD_NEXT, #A); if(box_##A == (B##_t)A) box_##A = NULL;
    SUPER()
    #undef GO2
    #undef GO
    #endif
}

#undef SUPER