#include "Request.hpp"

Request::Request(){
};

Request::Request(const Request& other){
  *this = other; 
};

Request& Request::operator=(const Request& other){
  if (this != &other) {
  }
  return *this;
};

Request::~Request(){};
