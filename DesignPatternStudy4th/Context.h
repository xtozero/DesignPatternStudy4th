#pragma once

#include <string>

class Context
{
public:
	std::string Get( ) const noexcept { return m_context; }

	explicit Context( const std::string& expression ) : m_context( expression ) {}
private:
	std::string m_context;
};