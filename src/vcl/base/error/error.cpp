#include "error.hpp"


#include <sstream>


#ifndef _WIN32
namespace external_backtrace_C{
#include <execinfo.h> // for backtrace
}
#include <dlfcn.h>    // for dladdr
#include <cxxabi.h>   // for __cxa_demangle
#endif

#include <iostream>



namespace vcl
{

void call_error(std::string assert_arg, std::string message, std::string filename, std::string function_name, int line, std::string const& trace)
{
    std::cerr<<"\n==========================================="<<std::endl;
    std::cerr<<"ERROR detected !\n"<<std::endl;
    std::cerr<<"Stacktrace:"<<std::endl;
    std::cerr<<trace<<std::endl;
    std::cerr<<"Error description:"<<std::endl;
    std::cerr<<"-------------------------------------"<<std::endl;
    std::cerr<<"  - File: "<<filename<<std::endl;
    std::cerr<<"  - Line: "<<line<<std::endl;
    std::cerr<<"  - Function: "<<function_name<<std::endl;
    if(assert_arg!="")
        std::cerr<<"  - Assert failed at condition: "<<assert_arg<<std::endl;
    if(message!="")
        std::cerr<<"  - Error message: "<<message<<std::endl;
    std::cerr<<std::endl;

    std::exit(EXIT_FAILURE);
}

#ifndef _WIN32
std::string backtrace(int const skip)
{
    void *callstack[128];
    int const nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
    char buf[1024];
    int const nFrames = external_backtrace_C::backtrace(callstack, nMaxFrames);
    char **symbols = external_backtrace_C::backtrace_symbols(callstack, nFrames);

    std::ostringstream trace_buf;
    for (int i = skip; i < nFrames; i++) {

        Dl_info info;
        if (dladdr(callstack[i], &info) && info.dli_sname) {
            char *demangled = nullptr;
            int status = -1;
            if (info.dli_sname[0] == '_')
                demangled = abi::__cxa_demangle(info.dli_sname, nullptr, 0, &status);
            snprintf(buf, sizeof(buf), "%-3d %*p %s + %zd\n",
                     i, int(2 + sizeof(void*) * 2), callstack[i],
                     status == 0 ? demangled :
                                   info.dli_sname == 0 ? symbols[i] : info.dli_sname,
                     static_cast<char *>(callstack[i]) - static_cast<char *>(info.dli_saddr));
            free(demangled);
        } else {
            snprintf(buf, sizeof(buf), "%-3d %*p %s\n",
                     i, int(2 + sizeof(void*) * 2), callstack[i], symbols[i]);
        }
        trace_buf << buf;
    }
    free(symbols);
    if (nFrames == nMaxFrames)
        trace_buf << "[truncated]\n";
    return trace_buf.str();
}
#endif

}


