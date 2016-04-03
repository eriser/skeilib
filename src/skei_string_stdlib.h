#ifndef skei_string_stdlib_included
#define skei_string_stdlib_included
//----------------------------------------------------------------------

#ifdef SKEI_REPLACE_STDLIB

//----------------------------------------------------------------------

#undef SStrcat
#undef SStrchr
#undef SStrcmp
#undef SStrcpy
//#undef SStrdup
#undef SStrlen
#undef SStrncat
//#undef SStrncmp
#undef SStrncpy
#undef SStrrchr
#undef SStrrev
#undef SStrstr

//----------------------------------------------------------------------

static __SKEI_UNUSED
const char* SStrcat(char* s1, const char* s2) {
  while (*s1) s1++;
  while ((*s1++ = *s2++));
  return s1;
}

//----------

//char* SStrchr (const char* str, const int c) {
//  while (*str && *str != c) str++;
//  if (*str != c) str = NULL;
//  return (char*) str;
//}

static __SKEI_UNUSED
const char *SStrchr(const char *str, int ch) {
  while (*str) {
    if (*str == ch) return str;
    str++;
  }
  return 0;
}

//----------

static __SKEI_UNUSED
int SStrcmp(const char* s1, const char* s2) {
  while (*s1 != '\0' && *s1 == *s2) {
    s1++;
    s2++;
  }
  return (*(unsigned char *) s1) - (*(unsigned char*) s2);
}

//----------

static __SKEI_UNUSED
char* SStrcpy(char* dest,const char* src) {
  while ((*dest++=*src++));
  return dest;
}

//----------

//char *SStrdup(const char *src) {
//  if (!src) return 0;
//  char *dst = (char*)malloc(strlen(src)+1);
//  strcpy(dst, src);
//  return dst;
//}

//----------

static __SKEI_UNUSED
unsigned int SStrlen(const char* str) {
  unsigned int num = 0;
  while (*str++) num++;
  return num;
}

//----------

static __SKEI_UNUSED
char* SStrncat(char* s1, char* s2, unsigned int n) {
  char* s = s1;
  while (*s1) s1++;
  while ( n-- && (*s1++ = *s2++) ) if (n == 0) *s1 = '\0';
  return s;
}

//----------

//int strncmp(const char *s1, const char *s2, size_t n) {
//  if (!n) return 0;
//  const unsigned char *p1 = (const unsigned char*)s1;
//  const unsigned char *p2 = (const unsigned char*)s2;
//  for (size_t i = 0; i < n; i++) {
//    if (!p1[i] || p1[i] != p2[i]) return p1[i] - p2[i];
//  }
//  return 0;
//}

//----------

static __SKEI_UNUSED
char* SStrncpy(char* dest, const char* src, unsigned int n) {
  while (n-- && (*dest++=*src++) != '\0');
  return dest;
}

//----------

static __SKEI_UNUSED
char* SStrrchr(const char* s, /*int*/char c) {
  char* p = SKEI_NULL;
  while (*s++) if (*s == c) p = (char*)s;
  return p;
}

//----------

// http://rosettacode.org/wiki/Reverse_a_string#C
// This function assumes the passed pointer points to a valid, zero-terminated string

static __SKEI_UNUSED
void SStrrev(char *s) {
  char *t = s;
  while (*t != '\0') t++;
  while (s < t) {
    int c = *s;
    *s++ = *--t;
    *t = c;
  }
}

//----------

static __SKEI_UNUSED
const char *SStrstr(/*const*/ char *str, /*const*/ char *substr) {
  int str_len = SStrlen(str);
  int substr_len = SStrlen(substr);
  if (substr_len == 0) return str;
  if (str_len < substr_len) return 0;
  for (int i = 0; i < (int)(str_len-substr_len+1); i++) {
    if (!SStrcmp(&str[i], substr)) return (const char *)(&str[i]);
  }
  return 0;
}

#endif // SKEI_REPLACE_STDLIB

//----------------------------------------------------------------------
#endif
