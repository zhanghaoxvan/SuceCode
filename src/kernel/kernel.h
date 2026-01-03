#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C
#endif

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#define IMPORT_API __declspec(dllimport)
#else
#define EXPORT_API
#define IMPORT_API
#endif

#ifdef BUILD_DLL
#define KERNELAPI EXTERN_C EXPORT_API
#else
#define KERNELAPI EXTERN_C IMPORT_API
#endif

struct Kernel;
struct KernelError {
    char *msg;
    char *type;
};

KERNELAPI const char *kernelVersion();

KERNELAPI struct Kernel *kernelCreate();
KERNELAPI void kernelDestroy(struct Kernel *kernel);
KERNELAPI struct KernelError *kernelDoString(struct Kernel *kernel, const char *chunk);
KERNELAPI struct KernelError *kernelDoStringWithName(struct Kernel *kernel, const char *chunk,
                                            const char *chunk_name);
KERNELAPI struct KernelError *kernelDoFile(struct Kernel *kernel, const char *filename);
