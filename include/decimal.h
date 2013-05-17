/////////////////////////////////////////////////////////////////////////////
// Name:        decimal.h
// Purpose:     Decimal data type support, for COBOL-like fixed-point
//              operations on currency values.
// Author:      Piotr Likus
// Modified by: Gustavo Barcena
// Modified:    Updated the code to change precision on the fly instead of 
//              of being templates and compile time. I would not suggest using 
//              it for precisions higher than 6
// Created:     03/01/2011
// Licence:     BSD
/////////////////////////////////////////////////////////////////////////////

#ifndef _DECIMAL_H__
#define _DECIMAL_H__

#include <cstdio>
#include <iostream>
#include <cstring>
#include <sstream>
#include <math.h>

using std::string;
// ----------------------------------------------------------------------------
// Description
// ----------------------------------------------------------------------------
/// \file decimal.h
///
/// Decimal value type. Use for capital calculations.
/// Note: maximum handled value is: +9,223,372,036,854,775,807 (divided by prec)
///
/// Sample usage:
///   using namespace dec;
///   decimal value(143125,2);
///   value = value / decimal(333,2);
///   cout << "Result is: " << value.getAsDouble() << endl;

namespace dec
{
    //For the future, to set the rounding type as desired.
    enum RoundingType 
    {
        BANKERS
    };
    
    // ----------------------------------------------------------------------------
    // Config section
    // ----------------------------------------------------------------------------
    // - define DEC_EXTERNAL_INT64 if you do not want internal definition of "int64" data type
    //   in this case define "DEC_INT64" somewhere
    // - define DEC_CROSS_DOUBLE if you want to use double (intead of xdouble) for cross-conversions
    
    // ----------------------------------------------------------------------------
    // Simple type definitions
    // ----------------------------------------------------------------------------
#define DEC_CROSS_DOUBLE

#ifndef DEC_EXTERNAL_INT64
#if defined(_MSC_VER) || defined(__BORLANDC__)
    typedef signed __int64 DEC_INT64;
#else
    typedef signed long long DEC_INT64;
#endif
#endif
    
    typedef DEC_INT64 int64;
    // type for storing currency value internally
    typedef int64 dec_storage_t;
    typedef unsigned int uint;
    // xdouble is an "extended double" - can be long double, __float128, _Quad - as you wish
    typedef long double xdouble;
    
    // ----------------------------------------------------------------------------
    // Forward class definitions
    // ----------------------------------------------------------------------------
    
    // ----------------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------------
    
    // ----------------------------------------------------------------------------
    // Class definitions
    // ----------------------------------------------------------------------------
    
    
    // round value - convert to int64
    inline int64 round(double value) {

        double val1, fractpart ,intpart;
        int64 intCheck = int64(value);

        fractpart = modf (value , &intpart);

        if(fractpart == 0.5)
        {
            double rounder = ((intCheck % 2) == 1) ? 0.5 : 0.4;
            if (value < 0.0)
                val1 = value - rounder;
            else
                val1 = value + rounder;  
        }else
        {
            if (value < 0.0)
                val1 = floor(value + 0.5);
            else
                val1 = ceil(value - 0.5);
        }

        int64 intPart = int64(val1);   
        return intPart;
    }
    
    inline int64 round(xdouble value) {
        xdouble val1;
        
        int64 intCheck = int64(value);
        double rounder = ((intCheck % 2) == 1) ? 0.5 : 0.4;
        if (value < 0.0)
            val1 = value - rounder;
        else
            val1 = value + rounder;        

        
        int64 intPart = int64(val1);
        return intPart;
    }
    
    class decimal 
    {
    public:
        typedef dec_storage_t raw_data_t;
        
#ifdef DEC_CROSS_DOUBLE
        typedef double cross_float;
#else
        typedef xdouble cross_float;
#endif
        
        decimal() { init(0,0); }
        decimal(const decimal &src) { init(src); }
        explicit decimal(uint value, int __precision) { init(value, __precision); }
        explicit decimal(int value, int __precision) { init(value, __precision); }
        explicit decimal(int64 value, int __precision) { init(value, __precision); }
        explicit decimal(xdouble value, int __precision, RoundingType roundingType) 
        { 
            init(value, __precision,  roundingType); 
        }
        explicit decimal(double value, int __precision, RoundingType roundingType) 
        { 
            init(value, __precision,  roundingType); 
        }
        explicit decimal(float value, int __precision, RoundingType roundingType) 
        { 
            init(value, __precision, roundingType); 
        }
        
        ~decimal() {}
        
        inline int getPrecision() const { return precision; }
        
        decimal & operator=(const decimal &rhs) 
        {
            if (&rhs != this) 
            {
                m_value = rhs.m_value;
                precision = rhs.precision;
                precisionFactor = rhs.precisionFactor;
            }
            return *this;
        }
    
        bool operator==(const decimal &rhs) const 
        {
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value;
            dec_storage_t lhs_m_value = m_value;
            
            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, lhs_m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);

            return (lhs_m_value == rhs_m_value);
        }
        bool operator<(const decimal &rhs) const 
        {   
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            dec_storage_t lhs_m_value = m_value;
            
            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, lhs_m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);
            
            return (lhs_m_value < rhs_m_value);
        }
        
        bool operator<=(const decimal &rhs) const 
        {
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            dec_storage_t lhs_m_value = m_value;
            
            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, lhs_m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);

            return (lhs_m_value <= rhs_m_value);
        }
        
        bool operator>(const decimal &rhs) const
        {
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            dec_storage_t lhs_m_value = m_value;

            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, lhs_m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);

            return (lhs_m_value > rhs_m_value);
        }
        
        bool operator>=(const decimal &rhs) const 
        {
             
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            dec_storage_t lhs_m_value = m_value;  

            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, lhs_m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);

            return (lhs_m_value >= rhs_m_value);
        }
        
        bool operator!=(const decimal &rhs) const 
        {
            return !(*this == rhs);
        }
        
        void precisionFunction(dec_storage_t &rhs_m_value, dec_storage_t &lhs_m_value, 
                                int rhs_precision, int rhs_precisionFactor,
                                int &precisionHighest, int &precisionHighestFactor,
                                int precisionDiff, int precisionDiffFactor) const
        {
            if (precision > rhs_precision) 
            {
                precisionDiff = precision - rhs_precision; 
                precisionDiffFactor = getPrecisionFactor(precisionDiff);
                rhs_m_value *= precisionDiffFactor;
            }
            else if (precision < rhs_precision) 
            {
                precisionHighest = rhs_precision;
                precisionHighestFactor = rhs_precisionFactor;
                precisionDiff = rhs_precision - precision;
                precisionDiffFactor = getPrecisionFactor(precisionDiff);
                lhs_m_value *= precisionDiffFactor;
            }
        }

        void precisionFunction2(dec_storage_t &rhs_m_value, const int precisionOut, int precisionHighest, int precisionDiff, int precisionDiffFactor)
        {
            precision = precisionOut;
            precisionFactor = getPrecisionFactor(precision);     
        
            if (precisionHighest > precisionOut) 
            {
                precisionDiff = precisionHighest - precisionOut;
                precisionDiffFactor = getPrecisionFactor(precisionDiff);
                m_value = round(static_cast<cross_float>(m_value)
                                /
                                static_cast<cross_float>(precisionDiffFactor));
            }
            else if (precisionHighest < precisionOut) 
            {
                precisionDiff = precisionOut - precisionHighest;
                precisionDiffFactor = getPrecisionFactor(precisionDiff);
                m_value = round(static_cast<cross_float>(m_value)
                                *
                                static_cast<cross_float>(precisionDiffFactor));
            }
        }
        
        void add(int rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut);
            this->add(x, precisionOut, roundingType);
        }
        
        void add(double rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut, roundingType);
            this->add(x, precisionOut, roundingType);
        }
        
        void add(float rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut, roundingType);
            this->add(x, precisionOut, roundingType);
        }
       
        void add(const decimal &rhs, const int precisionOut, RoundingType roundingType) 
        {

            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            
            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);
            
            m_value += rhs_m_value;

            precisionFunction2(rhs_m_value, precisionOut, precisionHighest, precisionDiff, precisionDiffFactor);
            
        }
        
        static const decimal add(const decimal &lhs, const decimal &rhs, const int precisionOut, RoundingType roundingType)  
        {
            decimal result = lhs;
            result.add(rhs,precisionOut, roundingType);
            return result;
        }
        
        void subtract(int rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut);
            this->subtract(x, precisionOut, roundingType);
        }
        
        void subtract(double rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut, roundingType);
            this->subtract(x, precisionOut, roundingType);
        }
        
        void subtract(float rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut, roundingType);
            this->subtract(x, precisionOut, roundingType);
        }
        
        void subtract(const decimal &rhs, const int precisionOut, RoundingType roundingType) 
        {

            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            int precisionHighestFactor = precisionFactor;
            precisionFunction(rhs_m_value, m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);

            m_value -= rhs_m_value;
            
            precisionFunction2(rhs_m_value, precisionOut, precisionHighest, precisionDiff, precisionDiffFactor);

        }
        
        static const decimal subtract(const decimal &lhs, const decimal &rhs, const int precisionOut, RoundingType roundingType)  
        {
            decimal result = lhs;
            result.subtract(rhs,precisionOut, roundingType);
            return result;
        }
        
        void multiply(int rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut);
            this->multiply(x, precisionOut, roundingType);
        }
        
        void multiply(double rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut, roundingType);
            this->multiply(x, precisionOut, roundingType);
        }
        
        void multiply(float rhs, const int precisionOut, RoundingType roundingType) 
        {
            decimal x( rhs, precisionOut, roundingType);
            this->multiply(x, precisionOut, roundingType);
        }
        
        void multiply(const decimal &rhs, const int precisionOut, RoundingType roundingType) 
        {
            int precisionHighestFactor = precisionFactor;
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            
            precisionFunction(rhs_m_value, m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);
            
            m_value =
            round(
                  static_cast<cross_float>(m_value * rhs_m_value)
                  /
                  static_cast<cross_float>(precisionHighestFactor)
                  );
            precisionFunction2(rhs_m_value, precisionOut, precisionHighest, precisionDiff, precisionDiffFactor);

        }

        static const decimal multiply(const decimal &lhs, const int &rhs, const int precisionOut, RoundingType roundingType)  
        {
            decimal result = lhs;
            result.multiply( rhs, precisionOut, roundingType);
            return result;
        }

        static const decimal multiply(const decimal &lhs, const double &rhs, const int precisionOut, RoundingType roundingType)  
        {
            decimal result = lhs;
            result.multiply( rhs, precisionOut, roundingType);
            return result;
        }

        static const decimal multiply(const decimal &lhs, const float &rhs, const int precisionOut, RoundingType roundingType)  
        {
            decimal result = lhs;
            result.multiply( rhs, precisionOut, roundingType);
            return result;
        }
        
        static const decimal multiply(const decimal &lhs, const decimal &rhs, const int precisionOut, RoundingType roundingType)  
        {
            decimal result = lhs;
            result.multiply(rhs,precisionOut, roundingType);
            return result;
        }
        
        void divide(int rhs, const int precisionOut, RoundingType roundingType) 
        {
            if(rhs == 0){           
                throw "It's not possible to divide by cero";
            }
            else{       
                decimal x( rhs, precisionOut);
                this->divide(x, precisionOut, roundingType);
            }
        }
        
        void divide(double rhs, const int precisionOut, RoundingType roundingType) 
        {
            if(rhs == 0){
                throw "It's not possible to divide by cero";
            }
            else{
                decimal x( rhs, precisionOut, roundingType);
                this->divide(x, precisionOut, roundingType);
            }
        }
        
        void divide(float rhs, const int precisionOut, RoundingType roundingType) 
        {
            if(rhs == 0){
                throw "It's not possible to divide by cero";
            }
            else{
                decimal x( rhs, precisionOut, roundingType);
                this->divide(x, precisionOut, roundingType);
            }
        }
        
        void divide(const decimal &rhs, const int precisionOut, RoundingType roundingType) 
        {
            int precisionHighestFactor = precisionFactor;
            
            int precisionHighest = precision;
            int precisionDiff = 0;
            int precisionDiffFactor = 0;
            dec_storage_t rhs_m_value = rhs.m_value; 
            
            if(&rhs == 0){
                throw "It's not possible to divide by cero";
            }
            else{
                precisionFunction(rhs_m_value, m_value, rhs.precision, rhs.precisionFactor , precisionHighest, precisionHighestFactor, precisionDiff, precisionDiffFactor);
            
                m_value =
                round(
                    static_cast<cross_float>(m_value * precisionHighestFactor)
                    /
                    static_cast<cross_float>(rhs_m_value)
                    );
            
                precisionFunction2(rhs_m_value, precisionOut, precisionHighest, precisionDiff, precisionDiffFactor);

            }
        }
        
        static const decimal divide(const decimal &lhs, const decimal &rhs, const int precisionOut, RoundingType roundingType)  
        {
            if(&lhs == 0){
                throw "It's not possible to divide by cero";
            }
            else{
                decimal result = lhs;
                result.divide(rhs,precisionOut, roundingType);
                return result;
            }
        }
        
        double getAsDouble() const 
        { 
            return static_cast<double>(m_value) / static_cast<double>(precisionFactor); 
        }
        
        
        xdouble getAsXDouble() const 
        { 
            return static_cast<xdouble>(m_value) / static_cast<xdouble>(precisionFactor); 
        }
                
        // returns integer value = real_value * (10 ^ precision)
        // use to load/store decimal value in external memory
        int64 getUnbiased() const { return m_value; }
        void setUnbiased(int64 value) { m_value = value; }
        
        //untested
        decimal abs(RoundingType roundingType) const 
        {
            if (m_value >= 0)
                return *this;
            else
                return (subtract(decimal(0,precision), *this, precision, roundingType));
        }
        
        int64 getAsInteger(RoundingType roundingType) const 
        {
            return round(getAsXDouble());
        }
        
        string toString()
        {
            dec_storage_t tempValue = m_value;
            int precisionTemp = precision;
            string toReturn = "";
            bool isNegative = false;
            
            int temp;
            std::ostringstream oss;
            
            if (tempValue < 0 ) 
            {
                isNegative = true;
                tempValue *= -1;
            }
            
            if (tempValue == 0 && precisionTemp == 0)
            {
                oss << "0";
            }

            while (tempValue > 0) 
            {
                temp = tempValue%10;
                tempValue = tempValue / 10;
                oss << temp;
                precisionTemp--;
                if (precisionTemp == 0) 
                {
                    oss << ".";
                }
                
            }
            
            while (precisionTemp > 0)
            {
                oss << "0";
                precisionTemp--;
                if (precisionTemp == 0)
                {
                    oss << ".";
                    oss << "0";
                }
            }
            if (isNegative) 
            {
                oss << "-";
            }
            toReturn = oss.str();
            toReturn = string ( toReturn.rbegin(), toReturn.rend() );
            return toReturn;
        }
        
    protected:
        
        void init(const decimal &src) 
        { 
            m_value = src.m_value; 
            precision = src.precision; 
            precisionFactor = src.precisionFactor; 
        }
        
        void init(uint value, int _precision) 
        { 
            precision = _precision;
            precisionFactor = getPrecisionFactor(precision);
            m_value = precisionFactor * static_cast<int>(value); 
        }
        
        void init(int value, int _precision) 
        { 
            precision = _precision;
            precisionFactor = getPrecisionFactor(precision);
            m_value = precisionFactor * value; 
        }
        
        void init(int64 value, int _precision) 
        { 
            precision = _precision;
            precisionFactor = getPrecisionFactor(precision);
            m_value = precisionFactor * value; 
        }
        
        void init(xdouble value, int _precision, RoundingType roundingType) 
        {
            precision = _precision;
            precisionFactor = getPrecisionFactor(precision);
            m_value =
            round(
                  static_cast<xdouble>(precisionFactor) *
                  value
                  );
        }
        
        void init(double value, int _precision, RoundingType roundingType) 
        {
            precision = _precision;
            precisionFactor = getPrecisionFactor(precision);
            m_value =
            round(
                  static_cast<double>(precisionFactor) *
                  value
                  );
        }
        
        void init(float value, int _precision, RoundingType roundingType) 
        {
            precision = _precision;
            precisionFactor = getPrecisionFactor(precision);
            m_value =
            round(
                  static_cast<double>(precisionFactor) *
                  static_cast<double>(value)
                  );
        }

    protected:
        dec_storage_t m_value;
        int precision;
        int precisionFactor;
        
        static int getPrecisionFactor(int prec)
        {
            if(prec <= 0 )
                return 1;
            else if (prec == 1)
                return 10;
            else
                return 10 * getPrecisionFactor(prec - 1);
        }
    };

    static const decimal ZERO = decimal();
    
} // namespace
#endif // _DECIMAL_H__
