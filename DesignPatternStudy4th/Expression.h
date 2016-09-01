#pragma once

#include "Context.h"

#include <type_traits>
#include <vector>

template <typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value || std::is_same <T, std::string>::value>::type>
class IExpression
{
public:
	virtual T Interpret( const Context& context ) = 0;
	
	virtual ~IExpression() = default;
};

/*
Terminal Expression
*/
template <typename T>
class CNumberExpr : public IExpression<T>
{
public:
	virtual T Interpret( const Context& ) override
	{ 
		return m_operand; 
	}
	explicit CNumberExpr( const T value ) noexcept : m_operand( value ) {}

private:
	T m_operand;
};

/*
Nonterminal Expression
*/
template <typename T>
class CPlusExpr : public IExpression<T>
{
public:
	virtual T Interpret( const Context& context ) override
	{ 
		return m_lhs.Interpret( context ) + m_rhs.Interpret( context );
	}
	
	CPlusExpr( IExpression<T>& lhs, IExpression<T>& rhs ) noexcept : m_lhs( lhs ), m_rhs( rhs ) {}

private:
	IExpression<T>& m_lhs;
	IExpression<T>& m_rhs;
};

template <typename T>
class CMinusExpr : public IExpression<T>
{
public:
	virtual T Interpret( const Context& context ) override
	{
		return m_lhs.Interpret( context ) - m_rhs.Interpret( context );
	}
	CMinusExpr( IExpression<T>& lhs, IExpression<T>& rhs ) noexcept : m_lhs( lhs ), m_rhs( rhs ) {}

private:
	IExpression<T>& m_lhs;
	IExpression<T>& m_rhs;
};

template <typename T>
class CMultiplyExpr : public IExpression<T>
{
public:
	virtual T Interpret( const Context& context ) override
	{
		return m_lhs.Interpret( context ) * m_rhs.Interpret( context );
	}
	CMultiplyExpr( IExpression<T>& lhs, IExpression<T>& rhs ) noexcept : m_lhs( lhs ), m_rhs( rhs ) {}

private:
	IExpression<T>& m_lhs;
	IExpression<T>& m_rhs;
};

template <typename T>
class CDivideExpr : public IExpression<T>
{
public:
	virtual T Interpret( const Context& context ) override
	{
		return m_lhs.Interpret( context ) / m_rhs.Interpret( context );
	}
	CDivideExpr( IExpression<T>& lhs, IExpression<T>& rhs ) noexcept : m_lhs( lhs ), m_rhs( rhs ) {}

private:
	IExpression<T>& m_lhs;
	IExpression<T>& m_rhs;
};