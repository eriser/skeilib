#ifndef skei_debug_test_included
#define skei_debug_test_included
//----------------------------------------------------------------------

#ifdef SKEI_DEBUG_TEST


// based on sput
// http://www.lingua-systems.com/unit-testing/
// Copyright (c) 2011-2015 Lingua-Systems Software GmbH
// BSD

//----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//----------------------------------------------------------------------
// definitions
//----------------------------------------------------------------------

#define SKEI_DEBUG_TEST_DEFAULT_SUITE_NAME   "Unlabeled Suite"
#define SKEI_DEBUG_TEST_DEFAULT_CHECK_NAME   "Unlabeled Check"
#define SKEI_DEBUG_TEST_INITIALIZED          0x06 /* ACK */

//----------------------------------------------------------------------
// skei_debug_test global variable
//----------------------------------------------------------------------

static struct skei_debug_test {
  char  initialized;
  struct skei_debug_test_overall {
    unsigned long checks;
    unsigned long suites;
    unsigned long ok;
    unsigned long nok;
  } overall;
  struct skei_debug_test_suite {
    const char   *name;
    unsigned long nr;
    unsigned long checks;
    unsigned long ok;
    unsigned long nok;
  } suite;
  struct skei_debug_test_test {
    const char   *name;
    unsigned long nr;
  } test;
  struct skei_debug_test_check {
    const char   *name;
    const char   *cond;
    const char   *type;
    unsigned long line;
  } check;
  struct skei_debug_test_time {
    time_t start;
    time_t end;
  } time;
} __skei_debug_test;

//----------------------------------------------------------------------
// skei_debug_test internal macros
//----------------------------------------------------------------------

#define _skei_debug_test_die_unless_initialized()                                                     \
  if (__skei_debug_test.initialized != SKEI_DEBUG_TEST_INITIALIZED)                                   \
  {                                                                                                   \
    DTrace("STest_StartTesting() omitted\n");                                                         \
    exit(EXIT_FAILURE);                                                                               \
  }

//----------

#define _skei_debug_test_die_unless_suite_set()                                                       \
  if (! __skei_debug_test.suite.name)                                                                 \
  {                                                                                                   \
    DTrace("STest_EnterSuite() omitted\n");                                                           \
    exit(EXIT_FAILURE);                                                                               \
  }

//----------

#define _skei_debug_test_die_unless_test_set()                                                        \
  if (! __skei_debug_test.test.name)                                                                  \
  {                                                                                                   \
    DTrace("STest_RunTest() omitted\n");                                                              \
    exit(EXIT_FAILURE);                                                                               \
  }

//----------

#define _skei_debug_test_check_failed()                                                               \
  {                                                                                                   \
    _skei_debug_test_die_unless_initialized();                                                        \
    _skei_debug_test_die_unless_suite_set();                                                          \
    __skei_debug_test.suite.nok++;                                                                    \
    DTrace( "  [%lu:%lu]  %s:#%lu  '%s'  FAIL\n"                                                      \
            "  - Type:      %s\n"                                                                     \
            "  - Condition: %s\n"                                                                     \
            "  - Line:      %lu\n",                                                                   \
            __skei_debug_test.suite.nr, __skei_debug_test.suite.checks, __skei_debug_test.test.name,  \
            __skei_debug_test.test.nr, __skei_debug_test.check.name, __skei_debug_test.check.type,    \
            __skei_debug_test.check.cond, __skei_debug_test.check.line);                              \
  }

//----------

#define _skei_debug_test_check_succeeded()                                                            \
  {                                                                                                   \
    _skei_debug_test_die_unless_initialized();                                                        \
    _skei_debug_test_die_unless_suite_set();                                                          \
    __skei_debug_test.suite.ok++;                                                                     \
    DTrace( "  [%lu:%lu]  %s:#%lu  '%s'  pass\n",                                                     \
            __skei_debug_test.suite.nr, __skei_debug_test.suite.checks,                               \
            __skei_debug_test.test.name,                                                              \
            __skei_debug_test.test.nr,                                                                \
            __skei_debug_test.check.name);                                                            \
  }

//----------------------------------------------------------------------
// user macros
//----------------------------------------------------------------------

#define STest_StartTesting()                                                                          \
  do {                                                                                                \
    memset(&__skei_debug_test, 0, sizeof(__skei_debug_test));                                         \
    __skei_debug_test.time.start  = time(NULL);                                                       \
    __skei_debug_test.initialized = SKEI_DEBUG_TEST_INITIALIZED;                                      \
    DTrace("\n--------------------------------------------------\n");                                 \
    DTrace("STest\n");                                                                                \
    DTrace("--------------------------------------------------\n\n");                                 \
  } while (0)

//----------

#define STest_LeaveSuite()                                                                            \
  do {                                                                                                \
    float failpls = 0.0f;                                                                             \
    _skei_debug_test_die_unless_initialized();                                                        \
    _skei_debug_test_die_unless_suite_set();                                                          \
    failpls = __skei_debug_test.suite.checks ? (float)                                                \
              ((__skei_debug_test.suite.nok * 100.0) / __skei_debug_test.suite.checks) :              \
              0.0f;                                                                                   \
    DTrace( "\n  %lu check(s), %lu ok, %lu failed (%.2f%%)\n\n",                                      \
            __skei_debug_test.suite.checks, __skei_debug_test.suite.ok, __skei_debug_test.suite.nok,  \
            failpls);                                                                                 \
    __skei_debug_test.overall.checks += __skei_debug_test.suite.checks;                               \
    __skei_debug_test.overall.ok     += __skei_debug_test.suite.ok;                                   \
    __skei_debug_test.overall.nok    += __skei_debug_test.suite.nok;                                  \
    memset(&__skei_debug_test.suite, 0, sizeof(__skei_debug_test.suite));                             \
  } while (0)

//----------

#define STest_GetReturnValue()                                                                        \
  (__skei_debug_test.overall.nok > 0 ? EXIT_FAILURE : EXIT_SUCCESS)

//----------

#define STest_EnterSuite(_name)                                                                       \
  do {                                                                                                \
    _skei_debug_test_die_unless_initialized();                                                        \
    if (__skei_debug_test.suite.name)                                                                 \
    {                                                                                                 \
      STest_LeaveSuite();                                                                             \
    }                                                                                                 \
    __skei_debug_test.suite.name = _name != NULL ?                                                    \
                        _name : SKEI_DEBUG_TEST_DEFAULT_SUITE_NAME;                                   \
    __skei_debug_test.suite.nr = ++__skei_debug_test.overall.suites;                                  \
    DTrace("* Entering suite #%lu:\n  '%s'\n\n",                                                      \
            __skei_debug_test.suite.nr, __skei_debug_test.suite.name);                                \
  } while (0)

//----------

#define STest_FinishTesting()                                                                         \
  do {                                                                                                \
    float failpft = 0.0f;                                                                             \
    _skei_debug_test_die_unless_initialized();                                                        \
    if (__skei_debug_test.suite.name)                                                                 \
    {                                                                                                 \
      STest_LeaveSuite();                                                                             \
    }                                                                                                 \
    failpft = __skei_debug_test.overall.checks ? (float)                                              \
              ((__skei_debug_test.overall.nok * 100.0) / __skei_debug_test.overall.checks) :          \
              0.0f;                                                                                   \
    __skei_debug_test.time.end = time(NULL);                                                          \
  DTrace("  ----------\n\n");                                                                         \
  DTrace( "  %lu check(s) in %lu suite(s) finished after %.2f second(s),\n"                           \
          "  %lu succeeded, %lu failed (%.2f%%)\n"                                                    \
          "\n  [%s]\n\n",                                                                             \
          __skei_debug_test.overall.checks, __skei_debug_test.overall.suites,                         \
          difftime(__skei_debug_test.time.end, __skei_debug_test.time.start),                         \
          __skei_debug_test.overall.ok, __skei_debug_test.overall.nok, failpft,                       \
          (STest_GetReturnValue() == EXIT_SUCCESS) ?                                                  \
          "SUCCESS" : "FAILURE");                                                                     \
  DTrace("--------------------------------------------------\n");                                     \
  } while (0);

//----------

#define STest_FailIf(_cond, _name)                                                                    \
  do {                                                                                                \
    _skei_debug_test_die_unless_initialized();                                                        \
    _skei_debug_test_die_unless_suite_set();                                                          \
    _skei_debug_test_die_unless_test_set();                                                           \
    __skei_debug_test.check.name = _name != NULL ?                                                    \
                        _name : SKEI_DEBUG_TEST_DEFAULT_CHECK_NAME;                                   \
    __skei_debug_test.check.line = __LINE__;                                                          \
    __skei_debug_test.check.cond = #_cond;                                                            \
    __skei_debug_test.check.type = "fail-if";                                                         \
    __skei_debug_test.test.nr++;                                                                      \
    __skei_debug_test.suite.checks++;                                                                 \
    if ((_cond))                                                                                      \
    {                                                                                                 \
      _skei_debug_test_check_failed();                                                                \
    }                                                                                                 \
    else                                                                                              \
    {                                                                                                 \
      _skei_debug_test_check_succeeded();                                                             \
    }                                                                                                 \
  } while (0)

//----------

#define STest_FailUnless(_cond, _name)                                                                \
  do {                                                                                                \
    _skei_debug_test_die_unless_initialized();                                                        \
    _skei_debug_test_die_unless_suite_set();                                                          \
    _skei_debug_test_die_unless_test_set();                                                           \
    __skei_debug_test.check.name = _name != NULL ?                                                    \
                        _name : SKEI_DEBUG_TEST_DEFAULT_CHECK_NAME;                                   \
    __skei_debug_test.check.line = __LINE__;                                                          \
    __skei_debug_test.check.cond = #_cond;                                                            \
    __skei_debug_test.check.type = "fail-unless";                                                     \
    __skei_debug_test.test.nr++;                                                                      \
    __skei_debug_test.suite.checks++;                                                                 \
    if (! (_cond))                                                                                    \
    {                                                                                                 \
      _skei_debug_test_check_failed();                                                                \
    }                                                                                                 \
    else                                                                                              \
    {                                                                                                 \
      _skei_debug_test_check_succeeded();                                                             \
    }                                                                                                 \
  } while (0)

//----------

#define STest_RunTest(_func)                                                                          \
  do {                                                                                                \
    _skei_debug_test_die_unless_initialized();                                                        \
    _skei_debug_test_die_unless_suite_set();                                                          \
    memset(&__skei_debug_test.test, 0, sizeof(__skei_debug_test.test));                               \
    __skei_debug_test.test.name = #_func;                                                             \
    _func();                                                                                          \
  } while (0)

//----------

//#ifdef __cplusplus
//}
//#endif

#else // SKEI_DEBUG_TEST

  #define STest_StartTesting()            {}
  #define STest_LeaveSuite()              {}
  #define STest_GetReturnValue()          (0)
  #define STest_EnterSuite(_name)         {}
  #define STest_FinishTesting()           {}
  #define STest_FailIf(_cond, _name)      {}
  #define STest_FailUnless(_cond, _name)  {}
  #define STest_RunTest(_func) {}

#endif // SKEI_DEBUG_TEST

//----------------------------------------------------------------------
#endif
