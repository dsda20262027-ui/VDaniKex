#ifndef VDANIKEX_H
#define VDANIKEX_H

#include <stdint.h>

#ifdef _WIN32
  #ifdef VDANIKEX_BUILD_DLL
    #define VDK_API __declspec(dllexport)
  #else
    #define VDK_API __declspec(dllimport)
  #endif
#else
  #define VDK_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define VDK_API_VERSION 1u

/* Returns the API version implemented by this scaffold. */
VDK_API uint32_t VDK_GetApiVersion(void);

/* Copies a short diagnostic build label into caller-owned memory.
   Returns the number of characters required, including the NUL terminator. */
VDK_API uint32_t VDK_GetBuildLabel(char* buffer, uint32_t buffer_size);

#ifdef __cplusplus
}
#endif

#endif /* VDANIKEX_H */
