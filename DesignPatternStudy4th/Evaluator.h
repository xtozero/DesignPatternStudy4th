#pragma once

#include "Context.h"
#include "Handler.h"

#include <map>
#include <memory>
#include <vector>

int GetPrecedence( char symbol )
{
	switch ( symbol )
	{
	case '(':
		return 0;
		break;
	case '+':
	case '-':
		return 1;
		break;
	case '*':
	case '/':
		return 2;
		break;
	default:
		assert( false && "Invaild Symbol" );
		break;
	}

	return -1;
}

bool IsGreaterEqualPrecedence( char lhs, char rhs )
{
	return GetPrecedence( lhs ) <= GetPrecedence( rhs );
}

template <typename T>
class CEvaluator
{
public:
	CEvaluator( )
	{
		RegisterDefaultHandler( );
	}

	void RegisterDefaultHandler( )
	{
		m_exprHandlers.push_back( std::make_unique<CNumberHandler<T>>() );
		m_exprHandlers.push_back( std::make_unique<CVariableHandler<T>>( ) );
		m_exprHandlers.push_back( std::make_unique<COperatorHandler<T>>( ) );
	}

	T Evaluate( const std::string& infix, const Context<T>& ctx )
	{
		return PostfixEvaluate( ConvertInfixToPostfix( infix ), ctx );
	}

private:
	std::vector<std::string> ConvertInfixToPostfix( const std::string& infix )
	{
		std::vector<std::string> result;
		std::stack<char> operatorStack;
		std::string value;
		value.reserve( 256 );

		for ( auto c : infix )
		{
			switch ( c )
			{
			case ')':
				HandleCloseBracket( operatorStack, value, result );
				break;
			case '(':
				HandleOpenBracket( c, operatorStack, value, result );
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				HandleOperator( c, operatorStack, value, result );
				break;
			default:
				value += c;
				break;
			}
		}

		if ( value.size( ) > 0 )
		{
			result.emplace_back( value );
		}

		while ( operatorStack.size( ) > 0 )
		{
			result.emplace_back( std::string( 1, operatorStack.top( ) ) );
			operatorStack.pop( );
		}

		return result;
	}

	void HandleCloseBracket( std::stack<char>& operatorStack, std::string& value, std::vector<std::string>& result/*out*/ )
	{
		if ( value.size( ) > 0 )
		{
			result.emplace_back( value );
			value.clear( );
		}

		while ( operatorStack.size( ) > 0 )
		{
			char symbol = operatorStack.top( );
			operatorStack.pop( );

			if ( symbol == '(' )
			{
				break;
			}

			result.emplace_back( std::string( 1, symbol ) );
		}
	}

	void HandleOpenBracket( char c, std::stack<char>& operatorStack, std::string& value, std::vector<std::string>& result/*out*/ )
	{
		if ( value.size( ) > 0 )
		{
			result.emplace_back( value );
			value.clear( );
		}

		operatorStack.push( c );
	}

	void HandleOperator( char c, std::stack<char>& operatorStack, std::string& value, std::vector<std::string>& result/*out*/ )
	{
		if ( value.size( ) > 0 )
		{
			result.emplace_back( value );
			value.clear( );
		}

		while ( operatorStack.size( ) > 0 )
		{
			char symbol = operatorStack.top( );

			if ( IsGreaterEqualPrecedence( c, symbol ) )
			{
				result.emplace_back( std::string( 1, symbol ) );
				operatorStack.pop( );
			}
			else
			{
				break;
			}
		}

		operatorStack.push( c );
	}

	T PostfixEvaluate( const std::vector<std::string>& postfix, const Context<T>& ctx )
	{
		bool handled = false;
		ExpressionStack<T> exprs;

		for ( const auto& pattern : postfix )
		{
			handled = false;
			for ( auto&& handler = m_exprHandlers.begin(); handler != m_exprHandlers.end() && handled == false; ++handler )
			{
				handled = handler->get()->HandleRequest( pattern, exprs );
			}
			assert( handled && "pattern doesn't handled" );
		}

		assert( exprs.size( ) == 1 );
		return exprs.top( )->Interpret( ctx );
	}

	std::vector<std::unique_ptr<IHandler<T>>> m_exprHandlers;
};
