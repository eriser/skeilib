#ifndef skei_debug_callstack_included
#define skei_debug_callstack_included
//----------------------------------------------------------------------

/*

  based on:
	https://github.com/wc-duck/dbgtools
	Copyright (C) 2013- Fredrik Kihlander

	//----------

  MSVC      - callstack_symbols() require linking against Dbghelp.lib.
  GCC/Clang - callstack_symbols() require -rdynamic to be specified as
              link-flag to get valid symbols.

*/

#ifdef SKEI_DEBUG_CALLSTACK

//----------------------------------------------------------------------

struct callstack_symbol {
	const char*  func;    // name of function containing address of function.
	const char*  file;    // file where symbol is defined, might not work on all platforms.
	unsigned int line;    // line in file where symbol is defined, might not work on all platforms.
	unsigned int ofs;     // offset from start of function where call was made.
};

//----------

struct callstack_string_buffer {
  char*       out_ptr;
  const char* end_ptr;
};

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

const char* callstack_alloc_string( callstack_string_buffer* buf, const char* str, size_t str_len ) {
  char* res;
  if ((size_t)(buf->end_ptr-buf->out_ptr)<(str_len+1)) return "out of memory";
  res = buf->out_ptr;
  buf->out_ptr += str_len + 1;
  memcpy(res,str,str_len);
  res[str_len] = '\0';
  return res;
}

//----------------------------------------------------------------------
// linux
//----------------------------------------------------------------------

#ifdef SKEI_LINUX

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cxxabi.h>

//----------------------------------------------------------------------

FILE* run_addr2line( void** addresses, int num_addresses, char* tmp_buffer, size_t tmp_buf_len ) {
  const char addr2line_run_string[] = "addr2line -e /proc/%u/exe";
  size_t start = (size_t)snprintf(tmp_buffer,tmp_buf_len,addr2line_run_string,getpid());
  for (int i=0; i<num_addresses; ++i) {
    start += (size_t)snprintf(tmp_buffer+start,tmp_buf_len-start," %p",addresses[i]);
  }
  return popen(tmp_buffer,"r");
}

//----------

char* demangle_symbol( char* symbol, char* buffer, size_t buffer_size ) {
  int status;
  char* demangled_symbol = abi::__cxa_demangle( symbol, buffer, &buffer_size, &status );
  return status != 0 ? symbol : demangled_symbol;
}

//----------

/*
  Generate a callstack from the current location in the code.
  skip_frames:    number of frames to skip in output to addresses.
  addresses:      is a pointer to a buffer where to store addresses in callstack.
  num_addresses:  size of addresses.
  return:         number of addresses in callstack.
*/

int callstack(int skip_frames, void** addresses, int num_addresses) {
  ++skip_frames;
  void* trace[256];
  int fetched = backtrace(trace,num_addresses+skip_frames)-skip_frames;
  memcpy(addresses,trace+skip_frames,(size_t)fetched*sizeof(void*));
  return fetched;
}

//----------

/*
  Translate addresses from, for example, callstack to symbol-names.
  addresses:      list of pointers to translate.
  out_syms:       list of callstack_symbol_t to fill with translated data, need to fit as many strings as there are ptrs in addresses.
  num_addresses:  number of addresses in addresses
  memory:         memory used to allocate strings stored in out_syms.
  mem_size:       size of addresses.
  return:         number of addresses translated.
*/

int callstack_symbols(void** addresses, callstack_symbol* out_syms, int num_addresses, char* memory, int mem_size) {
  int num_translated = 0;
  callstack_string_buffer outbuf = { memory, memory + mem_size };
  memset(out_syms,0x0,(size_t)num_addresses*sizeof(callstack_symbol));
  char** syms = backtrace_symbols(addresses,num_addresses);
  size_t tmp_buf_len = 1024 * 32;
  char*  tmp_buffer  = (char*)malloc(tmp_buf_len);
  FILE* addr2line = run_addr2line(addresses,num_addresses,tmp_buffer,tmp_buf_len);
  for (int i=0; i<num_addresses; ++i) {
    char* symbol = syms[i];
    unsigned int offset = 0;
    // find function name and offset
    char* name_start = strchr(symbol,'(');
    char* offset_start = name_start ? strchr(name_start,'+') : 0x0;
    if (name_start && offset_start) {
      // zero terminate all strings
      ++name_start;
      *offset_start = '\0';
      ++offset_start;
    }
    if (name_start && offset_start ) {
      offset = (unsigned int)strtoll(offset_start,0x0,16);
      symbol = demangle_symbol(name_start,tmp_buffer,tmp_buf_len);
    }
    out_syms[i].func  = callstack_alloc_string(&outbuf,symbol,strlen(symbol));
    out_syms[i].ofs   = offset;
    out_syms[i].file  = "failed to lookup file";
    out_syms[i].line  = 0;
    if( addr2line != 0x0 ) {
      if ( fgets(tmp_buffer,(int)tmp_buf_len, addr2line) != 0x0) {
        char* line_start = strchr(tmp_buffer,':');
        *line_start = '\0';
        if (tmp_buffer[0] != '?' && tmp_buffer[1] != '?') {
          out_syms[i].file = callstack_alloc_string(&outbuf,tmp_buffer,strlen(tmp_buffer));
        }
        out_syms[i].line = (unsigned int)strtoll(line_start+1,0x0,10);
      }
    }
    ++num_translated;
  }
  free(syms);
  free(tmp_buffer);
  fclose(addr2line);
  return num_translated;
}

#endif // SKEI_LINUX

//----------------------------------------------------------------------
// win32
//----------------------------------------------------------------------

#ifdef SKEI_WIN32

  #define WIN32_LEAN_AND_MEAN
  #define NOMINMAX
  #include <Windows.h>
  #include <Dbghelp.h>

  //----------

  int callstack( int skip_frames, void** addresses, int num_addresses ) {
    return RtlCaptureStackBackTrace( skip_frames + 1, num_addresses, addresses, 0 );
  }

  //----------

  int callstack_symbols( void** addresses, callstack_symbol_t* out_syms, int num_addresses, char* memory, int mem_size ) {
    HANDLE          process;
    DWORD64         offset;
    DWORD           line_dis;
    BOOL            res;
    IMAGEHLP_LINE64 line;
    PSYMBOL_INFO    sym_info;
    char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
    int num_translated = 0;
    callstack_string_buffer_t outbuf = { memory, memory + mem_size };
    memset( out_syms, 0x0, (size_t)num_addresses * sizeof(callstack_symbol_t) );
    process = GetCurrentProcess();
    res     = SymInitialize( process, NULL, TRUE ); // TODO: Only initialize once!
    if( res == 0 ) return 0;
    sym_info  = (PSYMBOL_INFO)buffer;
    sym_info->SizeOfStruct = sizeof(SYMBOL_INFO);
    sym_info->MaxNameLen   = MAX_SYM_NAME;
    line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
    for( int i = 0; i < num_addresses; ++i ) {
      res = SymFromAddr( process, (DWORD64)addresses[i], &offset, sym_info );
      if (res==0) out_syms[i].func = "failed to lookup symbol";
      else out_syms[i].func = callstack_alloc_string(&outbuf,sym_info->Name,sym_info->NameLen);
      res = SymGetLineFromAddr64( process, (DWORD64)addresses[i], &line_dis, &line );
      if( res == 0 ) {
        out_syms[i].ofs   = 0;
        out_syms[i].file  = "failed to lookup file";
        out_syms[i].line  = 0;
      }
      else {
        out_syms[i].ofs   = (unsigned int)line_dis;
        out_syms[i].file  = callstack_alloc_string( &outbuf, line.FileName, strlen( line.FileName ) );
        out_syms[i].line  = (unsigned int)line.LineNumber;
      }
      ++num_translated;
    }
    return num_translated;
  }

#endif // SKEI_WIN32

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

void skei_debug_callstack_print(void** AAddresses=SKEI_NULL, int ANumAddresses=0, int skip_last=0, int skip_first=0) {
  callstack_symbol  symbols[256];
  char              symbols_buffer[1024];
  int               num_addresses;
  if (AAddresses && (ANumAddresses>0)) {
    num_addresses = callstack_symbols(AAddresses,symbols,ANumAddresses,symbols_buffer,1024);
  }
  else {
    void* adr[256];
    int num = callstack(0,adr,256);
    num_addresses = callstack_symbols(adr,symbols,num,symbols_buffer,1024);
  }
  DTrace("\n----------------------------------------------------------------------\n");
  DTrace("callstack\n");
  DTrace("----------------------------------------------------------------------\n");

  /*
    the last two entries are
    - __libc_start_main
    - skeilib_debug()
  */
  num_addresses -= skip_last;

  for (int i=skip_first; i<num_addresses; i++) {
    DTrace("%3i : %s\n", i-skip_first, symbols[i].func);
    DTrace("      %s\n", symbols[i].file);
    DTrace("      line %i offset %i\n", symbols[i].line, symbols[i].ofs);
  }
  DTrace("----------------------------------------------------------------------\n\n");
}

/*
#define SDumpCallStackAll                     \
  {                                           \
    void* adr[256];                           \
    int num = callstack(0,adr,256);           \
    skei_debug_callstack_print(adr,num);      \
  }
*/

#define SDumpCallStack                        \
  {                                           \
    void* adr[256];                           \
    int num = callstack(0,adr,256);           \
    skei_debug_callstack_print(adr,num,2);    \
  }

#define SDumpCallStackSkip(s)                 \
  {                                           \
    void* adr[256];                           \
    int num = callstack(0,adr,256);           \
    skei_debug_callstack_print(adr,num,2,s);  \
  }


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#else // SKEI_DEBUG_CALLSTACK

  struct callstack_symbol {};
  struct callstack_string_buffer {};
  __SKEI_ALWAYS_INLINE int callstack(int skip_frames, void** addresses, int num_addresses) { return 0; }
  __SKEI_ALWAYS_INLINE int callstack_symbols(void** addresses, callstack_symbol* out_syms, int num_addresses, char* memory, int mem_size) { return 0; }
  __SKEI_ALWAYS_INLINE void SPrintCallStack(void** AAddresses=SKEI_NULL, int ANumAddresses=0) {}

  #define SDumpCallStack {}
  #define SDumpCallStackSkip(s) {}

#endif // SKEI_DEBUG_CALLSTACK

//----------------------------------------------------------------------
#endif




//----------------------------------------------------------------------
// see also
//----------------------------------------------------------------------

// http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes
// http://panthema.net/2008/0901-stacktrace-demangled/
// https://github.com/vmarkovtsev/DeathHandler
// https://github.com/Nanolat/c-callstack
