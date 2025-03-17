#ifndef IMAGE_H
#define IMAGE_H
#include <Header.h>
#include <iostream>
#include <vector>

class Image {
private:
  Header header;
  uint32_t width, height;
  std::vector<uint8_t> data; // It can be a data[] array if you want to
public:
  Image(uint32_t w, uint32_t h, uint64_t timestamp, std::string sensorName);
  uint8_t get(uint32_t i, uint32_t j);
  bool set(uint32_t i, uint32_t j, uint8_t value);
  uint32_t getWidth();
  uint32_t getHeight();
  uint64_t getTimestamp();
  std::string getSensorName();
  std::vector<uint8_t> serialize();
  bool deserialize(const std::vector<uint8_t> bytestream);
  static Header lazyload(const std::vector<uint8_t> bytestream);
  ~Image();
};
#endif
