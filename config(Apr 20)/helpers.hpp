#ifndef HELPERS_HPP
# define HELPERS_HPP

#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cstdlib>

void skipLeadingSpaces(std::string& str);
int countWords(const std::string &str);
void fillMap(std::map<std::string, std::string> &m, std::string s);
void fillMap(std::map<int, std::string>& m,
             const std::string& s,
             const std::vector<std::string>& codes);
void printMap(std::map<int, std::string> &m);
void printVector(const std::vector<std::string> &v);

#endif