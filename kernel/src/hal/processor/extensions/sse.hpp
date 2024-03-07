#ifndef PROCESSOR_SSE_EXTENSIONS
#define PROCESSOR_SSE_EXTENSIONS

namespace Processor
{
    namespace Extensions
    {   
        bool HasSSE();
        bool HasSSE2();
        bool HasSSSE3();
        bool HasSSE4_1();
        bool HasSSE4_2();
        bool HasSSE4A();

        void EnableSSE();
    };
};

#endif