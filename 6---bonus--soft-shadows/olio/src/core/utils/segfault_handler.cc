//! \file       segfault_handler.cc
//! \brief      Misc functions for catching and handling segfaults

#include "core/utils/segfault_handler.h"
#include <stdlib.h>
#include <signal.h>
#include <iostream>
#include <sstream>
#ifndef WIN32
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#endif

#include <spdlog/spdlog.h>

namespace olio {
namespace core {
namespace utils {

using namespace std;

#ifndef WIN32

// This function produces a stack backtrace with demangled function & method names.
std::string
Backtrace(int skip)
{
  void *callstack[128];
  const int nMaxFrames = sizeof(callstack) / sizeof(callstack[0]);
  char buf[1024];
  int nFrames = backtrace(callstack, nMaxFrames);
  char **symbols = backtrace_symbols(callstack, nFrames);

  std::ostringstream trace_buf;
  for (int i = skip; i < nFrames; i++) {
    // fprintf(stderr, "%s\n", symbols[i]);
    spdlog::critical(symbols[i]);

    Dl_info info;
    if (dladdr(callstack[i], &info) && info.dli_sname) {
      char *demangled = NULL;
      int status = -1;
      if (info.dli_sname[0] == '_')
        demangled = abi::__cxa_demangle(info.dli_sname, NULL, 0, &status);
      snprintf(buf, sizeof(buf), "%-3d %*p %s + %zd\n",
               i, int(2 + sizeof(void*) * 2), callstack[i],
               status == 0 ? demangled :
               info.dli_sname == 0 ? symbols[i] : info.dli_sname,
               (char *)callstack[i] - (char *)info.dli_saddr);
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


void
segfault_sigaction(int /*signal*/, siginfo_t *si, void * /*arg*/)
{
  // fprintf(stderr, "Caught segfault at address %p\n", si->si_addr);
  // fprintf(stderr, "trace:\n");
  // string trace = Backtrace(0);
  // cerr << trace << endl;
  spdlog::critical("Caught segfault at address {}\n", si->si_addr);
  spdlog::critical("trace:");
  string trace = Backtrace(0);
  spdlog::critical(trace);
  exit(0);
}


void
InstallSegfaultHandler()
{
  // install segfault handler
  struct sigaction sa;
  memset(&sa, 0, sizeof(struct sigaction));
  sigemptyset(&sa.sa_mask);
  sa.sa_sigaction = segfault_sigaction;
  sa.sa_flags   = SA_SIGINFO;
  sigaction(SIGSEGV, &sa, NULL);
}

#else

std::string
Backtrace(int /*skip*/)
{
  return string("");
}


void
InstallSegfaultHandler()
{
}

#endif

}  // namespace utils
}  // namespace core
}  // namespace olio
