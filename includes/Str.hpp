#ifndef STR_HPP
#define STR_HPP

#include <string>
#include <vector>
#include <map>


class Str
{
    public:
		std::string *p;

    public:
		Str();
		Str(Str const &other);
		Str& operator=(Str const &rhs);
		~Str();
};

#endif
