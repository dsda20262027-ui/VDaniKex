#include "vdanikex.h"

#include <cstring>

extern "C" VDK_API uint32_t VDK_GetApiVersion(void)
{
    return VDK_API_VERSION;
}

extern "C" VDK_API uint32_t VDK_GetBuildLabel(char* buffer, uint32_t buffer_size)
{
    static const char label[] = "VDaniKex 0.1.0-dev (scaffold only)";
    const uint32_t required = static_cast<uint32_t>(sizeof(label));
    if (buffer != nullptr && buffer_size > 0) {
        const size_t copy_size = (buffer_size < required) ? buffer_size : required;
        std::memcpy(buffer, label, copy_size);
        buffer[buffer_size - 1] = '\0';
    }
    return required;
}
