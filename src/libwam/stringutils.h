#ifndef STRING_UTILS_HEADER
#define STRING_UTILS_HEADER

#include <string>
#include <memory>
#include <iostream>
#include <cstdio>
#include <sstream>
#include <vector>

void ltrim(std::string &s);
void rtrim(std::string &s);
void trim(std::string &s);

std::string ltrim_copy(std::string s);
std::string rtrim_copy(std::string s);
std::string trim_copy(std::string s);


template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    std::size_t size = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    std::unique_ptr<char[]> buf( new char[ size ] ); 
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}

template<class T>
T from_string( const std::string& Str)
{
	T Dest;
    std::istringstream iss( Str );
    iss >> Dest;
	return Dest;
}

/* 
template<typename T>
std::string to_string( const T & Value )
{
    std::ostringstream oss;
    oss << Value;
    return oss.str();
}*/

template<>
double from_string( const std::string& Str);

template<>
float from_string( const std::string & Str);

template<>
int from_string( const std::string & Str);

template<>
std::string from_string(const std::string & Str);



template<class T, class IteratorOut>
IteratorOut itemize(const std::string& txt, IteratorOut itemIt, const char delim = ' ') {
	std::string dummy;
	std::stringstream istr( txt);

	while(! istr.eof()) {
		getline(istr, dummy, delim);
		if( !dummy.empty()) {
			*itemIt++ = from_string<T>(dummy);
		}
	}

	return itemIt;

}

template<class T, class IteratorOut>
IteratorOut split(const std::string& txt, IteratorOut itemIt, int n=-1) {
	T dummy;
	std::stringstream istr( txt);

	while(n!=0) {
		istr >> dummy;
		if( istr.fail())
			break;
		*itemIt++ = dummy;
		--n;
	}
	if(!istr.fail()) {
		std::stringstream notread;
		notread << istr.rdbuf();
		*itemIt++ = notread.str();
	}
		
		//*itemIt++ = istr.str().substr( istr.tellg() );
	return itemIt;
}

template<class IteratorOut>
IteratorOut splitTxt(const std::string& txt, IteratorOut itemIt, const char *delimiteur = " ",  int n=-1) {
    std::string token, buffer(txt);
    while(token != buffer && n != 0 ){
        token = buffer.substr(0,buffer.find_first_of(delimiteur));
        buffer = buffer.substr(buffer.find_first_of(delimiteur) + 1);
        *itemIt++ = token;
        n--;
    }
    if( token != buffer) *itemIt++ = buffer;
    return itemIt;
}


template<class T, class U>
bool itemize(const std::string& txt, T& t, U& u) {
	std::stringstream istr( txt);
	istr >> t >> u;
	return ! istr.fail();
}

template<class T, class U, class V>
bool itemize(const std::string& txt, T& t, U& u, V& v) {
	std::stringstream istr( txt);
	istr >> t >> u >> v;
	return ! istr.fail();
}

template<class T, class U, class V, class W>
bool itemize(const std::string& txt, T& t, U& u, V& v, W& w) {
	std::stringstream istr( txt);
	istr >> t >> u >> v >> w;
	return ! istr.fail();
}

#endif