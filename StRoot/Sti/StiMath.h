#ifndef StiMath_H_INCLUDED
#define StiMath_H_INCLUDED
class StiMath
{
 public:
  virtual ~StiMath(){};
  static double chi2(double x, int n);
  static double gamma(double x);
  static double logGamma(double x);
  static double factorial(int n);
  static double logFactorial(int n);
  static double binomial(int n, int k);
 protected:
  StiMath(){};
};

#endif
