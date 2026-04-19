#ifndef HELPERS_HPP
# define HELPERS_HPP

#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <vector>

void skipLeadingSpaces(std::string& str);
int countWords(const std::string &str);
void fillMap(std::map<std::string, std::string> &m, std::string s);
void fillMap(std::map<int, std::string> &m, std::string s, size_t startIndex);
void printMap(const std::map<int, std::string> &m);
void printVector(const std::vector<std::string> &v);

#endif