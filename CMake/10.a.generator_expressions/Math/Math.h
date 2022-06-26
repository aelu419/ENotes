#ifndef Math_h
#define Math_h

#if defined(_WIN32)
#  if defined(EXPORTING_MATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else
#  define DECLSPEC
#endif

namespace CustomMath {
    double DECLSPEC exp(double);
    double DECLSPEC print_generated();
}

#endif