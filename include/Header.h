#include <fstream>
#include <iostream>
#ifndef HEADER_H
#define HEADER_H
struct Header {
  uint64_t timestamp;
  std::string sensor_name;
  uint64_t offset_position = 0; // could be /**std::streamoff*/
};
#endif
