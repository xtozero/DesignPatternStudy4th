#define CATCH_CONFIG_MAIN

#include "Expression.h"
#include "Lib\catch.hpp"

#include <iostream>
#include <type_traits>

TEST_CASE( "Expression" )
{
	Context dummyContext("1+2+3");

	SECTION( "Operand Expression" )
	{
		float floatingInput = 3.14f;
		CNumberExpr<float> floatingType( floatingInput );

		REQUIRE( floatingType.Interpret( dummyContext ) == floatingInput );

		int integerInput = 20160911;
		CNumberExpr<int> integerType( integerInput );

		REQUIRE( integerType.Interpret( dummyContext ) == integerInput );
	}

	int lhsInput = 20160911;
	int rhsInput = 11906102;
	CNumberExpr<int> lhs( 20160911 );
	CNumberExpr<int> rhs( 11906102 );

	SECTION( "Plus Expression" )
	{
		CPlusExpr<int> plus( lhs, rhs );
		REQUIRE( plus.Interpret( dummyContext ) == lhsInput + rhsInput );

		CPlusExpr<int> rPlus( rhs, lhs );
		REQUIRE( rPlus.Interpret( dummyContext ) == rhsInput + lhsInput );
	}

	SECTION( "Minus Expression" )
	{
		CMinusExpr<int> minus( lhs, rhs );
		REQUIRE( minus.Interpret( dummyContext ) == lhsInput - rhsInput );

		CMinusExpr<int> rMinus( rhs, lhs );
		REQUIRE( rMinus.Interpret( dummyContext ) == rhsInput - lhsInput );
	}

	SECTION( "Multiply Expression" )
	{
		CMultiplyExpr<int> mul( lhs, rhs );
		REQUIRE( mul.Interpret( dummyContext ) == lhsInput * rhsInput );

		CMultiplyExpr<int> rMul( rhs, lhs );
		REQUIRE( mul.Interpret( dummyContext ) == rhsInput * lhsInput );
	}

	SECTION( "Divide Expression" )
	{
		CDivideExpr<int> div( lhs, rhs );
		REQUIRE( div.Interpret( dummyContext ) == lhsInput / rhsInput );

		CDivideExpr<int> rDiv( rhs, lhs );
		REQUIRE( rDiv.Interpret( dummyContext ) == rhsInput / lhsInput );
	}
}