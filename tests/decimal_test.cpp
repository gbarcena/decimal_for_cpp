/*
 * Purpose: Unit test to exercise Decimal class as well as inherited classes
 * Author: Laura Vallejo <lauravallejoc@gmail.com>
 * Date:    20-7-2012
 *
 */
 
#include "Decimal.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <math.h>


#define BOOST_TEST_MODULE PocoPOS test suite
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test_framework.hpp>

using std::cout;
using namespace dec;
using namespace std;

BOOST_AUTO_TEST_SUITE( decimal_test_suite )

// //ADD ---->  decimal numbers
BOOST_AUTO_TEST_CASE( add_test_1 ) {

	decimal a(1.0,3, BANKERS);
	a.add(2.2,3,BANKERS);	
	decimal b(3.2,3, BANKERS);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//ADD ----> integer numbers
BOOST_AUTO_TEST_CASE( add_test_2 ) {
	
 	decimal a(1,3);
	a.add(2,3,BANKERS);	
	decimal b(3,3);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//SUBTRACT ---> decimal numbers
BOOST_AUTO_TEST_CASE( subtract_test_1 ) {
	
    decimal a(1.3,3, BANKERS);
	a.subtract(0.2,3, BANKERS);
	decimal b(1.1,3, BANKERS);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//SUBTRACT ---> integer numbers
BOOST_AUTO_TEST_CASE( subtract_test_2 ) {
	
    decimal a(456,3);
	a.subtract(430,3,BANKERS);
	decimal b(26,3);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//SUBTRACT ---> negavite numbers
BOOST_AUTO_TEST_CASE( subtract_test_3 ) {
	
    decimal a(450,3);
	a.subtract(456,3,BANKERS);	
	decimal b(-6,3);	
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//MULTIPLY ---> decimal by decimal
BOOST_AUTO_TEST_CASE( multiply_test_1 ) {

	decimal a(1.3,3, BANKERS);
	a.multiply(4.5,3,BANKERS);
	decimal b(5.85,3, BANKERS);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//MULTIPLY ---> integer by integer
BOOST_AUTO_TEST_CASE( multiply_test_2 ) {

	decimal a(15,3);
	a.multiply(45,3,BANKERS);
	decimal b(675,3);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
}

//MULTIPLY ---> number by 0
BOOST_AUTO_TEST_CASE( Multiplity_test_3 ) {
	
	decimal a(15,3);
	a.multiply(2,3,BANKERS);
	decimal b(30,3);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );

}

//DIVIDE ---> decimal by decimal
BOOST_AUTO_TEST_CASE( divide__test_1 ) {
	
	decimal a(1.3,3, BANKERS);
	a.divide(0.3,3,BANKERS);	
	decimal b(4.333,3, BANKERS);
	BOOST_CHECK_EQUAL( a.getAsDouble(),b.getAsDouble() );
	
}

//DIVIDE ---> number by 0
BOOST_AUTO_TEST_CASE( divide_test_2 ) {
	
	decimal a(1.3,3, BANKERS);

	BOOST_CHECK_THROW( a.divide(0,3,BANKERS), const char * );
}

//PRECISION --->
BOOST_AUTO_TEST_CASE( Testing_Rounding ) {
	
	decimal a0(35.550,2, BANKERS);
    decimal a1(35.551,2, BANKERS);
    decimal a2(35.552,2, BANKERS);
    decimal a3(35.553,2, BANKERS);
    decimal a4(35.554,2, BANKERS);
    decimal a5(35.555,2, BANKERS);
    decimal a6(35.556,2, BANKERS);
    decimal a7(35.557,2, BANKERS);
    decimal a8(35.558,2, BANKERS);
    decimal a9(35.559,2, BANKERS);
	decimal a10(35.560,2, BANKERS);
    decimal a11(35.561,2, BANKERS);
    decimal a12(35.562,2, BANKERS);
    decimal a13(35.563,2, BANKERS);
    decimal a14(35.564,2, BANKERS);
    decimal a15(35.565,2, BANKERS);
    decimal a16(35.566,2, BANKERS);
    decimal a17(35.567,2, BANKERS);
    decimal a18(35.568,2, BANKERS);
    decimal a19(35.569,2, BANKERS);
 	
	decimal b0(35.55,2, BANKERS);
    decimal b1(35.55,2, BANKERS);
	decimal b2(35.55,2, BANKERS);
	decimal b3(35.55,2, BANKERS);
	decimal b4(35.55,2, BANKERS);
	decimal b5(35.55,2, BANKERS);
	decimal b6(35.56,2, BANKERS);
	decimal b7(35.56,2, BANKERS);
	decimal b8(35.56,2, BANKERS);
	decimal b9(35.56,2, BANKERS);
	decimal b10(35.56,2, BANKERS);
	decimal b11(35.56,2, BANKERS);
	decimal b12(35.56,2, BANKERS);
	decimal b13(35.56,2, BANKERS);
	decimal b14(35.56,2, BANKERS);
	decimal b15(35.56,2, BANKERS);
	decimal b16(35.57,2, BANKERS);
	decimal b17(35.57,2, BANKERS);
	decimal b18(35.57,2, BANKERS);
	decimal b19(35.57,2, BANKERS);
	
	BOOST_CHECK_EQUAL( a0.getAsDouble(),b0.getAsDouble() );	
	BOOST_CHECK_EQUAL( a1.getAsDouble(),b1.getAsDouble() );
	BOOST_CHECK_EQUAL( a2.getAsDouble(),b2.getAsDouble() );
	BOOST_CHECK_EQUAL( a3.getAsDouble(),b3.getAsDouble() );
	BOOST_CHECK_EQUAL( a4.getAsDouble(),b4.getAsDouble() );
	BOOST_CHECK_EQUAL( a5.getAsDouble(),b5.getAsDouble() );
	BOOST_CHECK_EQUAL( a6.getAsDouble(),b6.getAsDouble() );
	BOOST_CHECK_EQUAL( a7.getAsDouble(),b7.getAsDouble() );
	BOOST_CHECK_EQUAL( a8.getAsDouble(),b8.getAsDouble() );
	BOOST_CHECK_EQUAL( a9.getAsDouble(),b9.getAsDouble() );
	BOOST_CHECK_EQUAL( a10.getAsDouble(),b10.getAsDouble() );
	BOOST_CHECK_EQUAL( a11.getAsDouble(),b11.getAsDouble() );
	BOOST_CHECK_EQUAL( a12.getAsDouble(),b12.getAsDouble() );
	BOOST_CHECK_EQUAL( a13.getAsDouble(),b13.getAsDouble() );
	BOOST_CHECK_EQUAL( a14.getAsDouble(),b14.getAsDouble() );
	BOOST_CHECK_EQUAL( a15.getAsDouble(),b15.getAsDouble() );
	BOOST_CHECK_EQUAL( a16.getAsDouble(),b16.getAsDouble() );
	BOOST_CHECK_EQUAL( a17.getAsDouble(),b17.getAsDouble() );
	BOOST_CHECK_EQUAL( a18.getAsDouble(),b18.getAsDouble() );
	BOOST_CHECK_EQUAL( a19.getAsDouble(),b19.getAsDouble() );
	
}

BOOST_AUTO_TEST_CASE( Testing_precision_2 ) {

	decimal a(0.3,3, BANKERS);
	
	a.subtract(0.099, 2, BANKERS);
    a.multiply(1000, 3, BANKERS);
	BOOST_CHECK_EQUAL( a.toString(), "200.000" );

}

BOOST_AUTO_TEST_CASE( Testing_decimal_class_by_double_2 ) {

	decimal subtotal(100.20,6, BANKERS);
    decimal tax(1.2345,6, BANKERS);
	decimal total(0,6);
    
    total = decimal::multiply(subtotal, tax, 6, BANKERS);
	
	BOOST_CHECK_EQUAL( total.getAsDouble(), 123.6969);
}

BOOST_AUTO_TEST_CASE( Testing_equivalente_2 ) {

    decimal pos(1.234, 4, BANKERS);
    decimal neg = pos;
    decimal negOne = decimal(-1.,2,BANKERS);
    
    neg.multiply(negOne, 4, BANKERS);
    
	BOOST_CHECK_EQUAL( pos.getAsDouble(), 1.234);
	BOOST_CHECK_EQUAL( negOne.getAsDouble(), -1.);
	BOOST_CHECK_EQUAL( neg.getAsDouble(), -1.234);
}

BOOST_AUTO_TEST_CASE( Testing_string ) {
    
	decimal stringer(1.234, 4, BANKERS);
    decimal stringer1(1234, 0);
    decimal stringer2(-1.234, 4, BANKERS);
    decimal stringer3(-123456789111., 6, BANKERS);
    
	BOOST_CHECK_EQUAL( stringer.getAsDouble(),1.234);
	BOOST_CHECK_EQUAL( stringer1.getAsDouble(),1234);
	BOOST_CHECK_EQUAL( stringer2.getAsDouble(),-1.234);
	BOOST_CHECK_EQUAL( stringer3.getAsDouble(),-123456789111.);

}

BOOST_AUTO_TEST_CASE( Testing_string_2 ) {

	decimal a(-1.234,4,BANKERS);
	decimal b(1.234,4,BANKERS);

	a = a.abs(BANKERS);
	b = b.abs(BANKERS);

	BOOST_CHECK_EQUAL( a.getAsDouble(),1.234);
	BOOST_CHECK_EQUAL( b.getAsDouble(),1.234);
}	

BOOST_AUTO_TEST_SUITE_END()

