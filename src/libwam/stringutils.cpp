#include <algorithm> 
#include <cctype>
#include <locale>
#include "stringutils.h"

using namespace std;

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

// trim from start (copying)
std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

template<>
double from_string( const std::string& Str) {
	return std::stod(Str);
}

template<>
float from_string( const std::string & Str) {
	return std::stof(Str);
}

template<>
int from_string( const std::string & Str) {
	return std::stoi(Str);
}

template<>
std::string from_string(const string & Str) {
	return Str;
}

template<>
bool itemize(const std::string& txt, float& t, float& u) {
	return sscanf(txt.c_str(), "%f%f", &t, &u) != EOF;
}

template<>
bool itemize(const std::string& txt, double& t, double& u) {
	return sscanf(txt.c_str(), "%lf%lf", &t, &u) != EOF;
}


template<>
bool itemize(const std::string& txt, float& t, float& u, float &v) {
	return sscanf(txt.c_str(), "%f%f%f", &t, &u, &v) != EOF;
}

template<>
bool itemize(const std::string& txt, double& t, double& u, double &v) {
	return sscanf(txt.c_str(), "%lf%lf%lf", &t, &u, &v) != EOF;
}


template<>
bool itemize(const std::string& txt, float& t, float& u, float &v, float &w) {
	return sscanf(txt.c_str(), "%f%f%f%f", &t, &u, &v, &w) != EOF;
}

template<>
bool itemize(const std::string& txt, double& t, double& u, double &v, double &w) {
	return sscanf(txt.c_str(), "%lf%lf%lf%lf", &t, &u, &v, &w) != EOF;
}
