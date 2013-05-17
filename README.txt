Purpose:     Decimal data type support, for COBOL-like fixed-point
             operations on currency/money values.
Author:      Piotr Likus
Created:     03/01/2011
Licence:     BSD

NOTE: These changes were orginally made from and older version of the code 
so some of the later features (like maximum ) are not implemented. 


ORIGINAL 
This data type is designed to perform calculation with on-fly  roundings
&  to  support  correct  compare  function  (floating-point  compare  is
unreliable).

Values are stored internally using 64-bit integer, so maximum number of
digits is 18.

Precision is user-defined, so you can use this data  type  for  currency
rates.

To store decimal in file you can use "unbiased" functions.

UPDATED
The code has been changed to no longer use templates so that the precision
does not have to be known at compile time. It also uses bankers rounding by 
default and can no longer use external rounding functions. These were 
requirements for a specific project. If these things dont matter, it is 
better to use the original implementation. 

Example usage:
  #include "decimal.h"

  using namespace dec;

  // the following declares currency variable with 2 decimal points
  // initialized with integer value (can be also floating-point)
  // and sets the rounding type to BANKER
  decimal value(143125.0, 2, BANKERS);

  // to use non-decimal constants you need to convert them to decimal
  value.divide(333.0, 2 , BANKERS);

  // output values
  cout << "Result is: " << value.toString() << endl;
  // this should display something like "429.80"

  // to mix decimals with different precision use decimal_cast
  decimal exchangeRate(12.1234, 6, BANKERS);
  value.multiply(exchangeRate, 2, BANKERS);

  cout << "Result 2 is: " << value.toString() << endl;
  // this should display something like "5210.64"

  decimal new_value = decimal::multiply(value, 1, 6, BANKERS);
  cout << "Result 2<6> is: " << new_value.toString() << endl;
  // this should display something like "5210.640000"


Directory structure:
\doc     - documentation
\include - headers
\test    - unit tests, Boost-based

