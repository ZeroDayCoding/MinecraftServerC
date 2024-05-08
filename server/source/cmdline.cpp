#include "cmdline.hpp"
#include <assert.h>

namespace ZDC
{
    
    CmdLine::CmdLine(int argc, const char** argv) : argc(argc), argv(argv) {
        assert(argc > 0 && "CmdLine constructed with an invalid number of arguments.");
        assert(argv && "CmdLine constructed with an invalid command line pointer.");

        // TODO: auto parsing for cmd arguments.
    }

} // namespace ZDC
