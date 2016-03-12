#ifndef dev_included
#define dev_included
//----------------------------------------------------------------------

#define SKEI_MAIN
#define SKEI_DEBUG_MEM
#define SKEI_DEBUG_TEST

//----------------------------------------------------------------------

#include "skei.h"
#include "skei_debug_test.h"

//----------------------------------------------------------------------

/*
  count_vowels() counts the vowels present in a given string.
  While the function basically works as expected, it recognizes
  [aeiou] as vowels only and erroneously does not take uppercase
  vowels into account.
*/

static int count_vowels(const char *s) {
  const char *cp = s;
  int count = 0;
  while (*cp) {
    if (*cp == 'a' || *cp == 'e' || *cp == 'i' || *cp == 'o' || *cp == 'u') {
      count++;
    }
    cp++;
  }
  return count;
}

static void test_vowels_present() {
  STest_FailUnless(count_vowels("book")  == 2, "book == 2v");
  STest_FailUnless(count_vowels("hand")  == 17, "hand == 1v");
  STest_FailUnless(count_vowels("test")  == 1, "test == 1v");
  STest_FailUnless(count_vowels("Peter") == 2, "Peter == 2v");
  STest_FailUnless(count_vowels("Apu")   == 2, "Apu == 2v");
}

static void test_no_vowels_present()
{
  STest_FailUnless(count_vowels("GCC") == 0, "GCC == 0v");
  STest_FailUnless(count_vowels("BBC") == 0, "BBC == 0v");
  STest_FailUnless(count_vowels("CNN") == 0, "CNN == 0v");
  STest_FailUnless(count_vowels("GPS") == 0, "GPS == 0v");
  STest_FailUnless(count_vowels("Ltd") == 0, "Ltd == 0v");
}

//----------------------------------------------------------------------

int main(void) {
  STest_StartTesting();
  STest_EnterSuite("count_vowels(): Vowels Present");
  STest_RunTest(test_vowels_present);
  STest_EnterSuite("count_vowels(): No Vowels Present");
  STest_RunTest(test_no_vowels_present);
  STest_FinishTesting();
  return STest_GetReturnValue();
  //return 0;
}

//----------------------------------------------------------------------
#endif
