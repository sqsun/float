#include "blocksize.h"
#include "spm.h"


#define EPS 1000*FLT_EPSILON

static inline bool samenum(const float x, const float y)
{
  return (fabsf(x-y) < EPS);
}

static inline bool isSym(const len_t n, const float *const restrict x)
{
  for (len_t j=0; j<n; j+=BLOCKSIZE)
  {
    for (len_t i=j; i<n; i+=BLOCKSIZE)
    {
      for (len_t col=j; col<j+BLOCKSIZE && col<n; ++col)
      {
        for (len_t row=i; row<i+BLOCKSIZE && row<n; ++row)
        {
          const bool check = samenum(x[col + n*row], x[row + n*col]);
          if (!check)
            return false;
        }
      }
    }
  }
  
  return true;
}



#define LGL(x) LOGICAL(x)[0]

SEXP R_isSymmetric_spm(SEXP x_ptr)
{
  SEXP ret;
  matrix_t *x = (matrix_t*) getRptr(x_ptr);
  const len_t m = NROWS(x);
  const len_t n = NCOLS(x);
  
  PROTECT(ret = allocVector(LGLSXP, 1));
  
  if (m != n)
  {
    LGL(ret) = 0;
  }
  else if (n == 0)
  {
    LGL(ret) = 1;
  }
  else
  {
    LGL(ret) = isSym(n, DATA(x));
  }
  
  UNPROTECT(1);
  return ret;
}