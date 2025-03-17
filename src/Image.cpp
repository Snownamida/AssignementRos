#include "../include/Image.h"
#include <cstdint>

Image::Image(uint32_t w, uint32_t h, uint64_t timestamp, std::string sensorName)
    : width(w), height(h) {
  header.timestamp = timestamp;
  header.sensor_name = sensorName;
  data.resize(w * h);
}

uint8_t Image::get(uint32_t i, uint32_t j) {

  if (i >= height || j >= width) {
    std::cerr << "Error: Index out of bounds!" << std::endl;
    return 0;
  }

  return data[i * width + j];
}
bool Image::set(uint32_t i, uint32_t j, uint8_t value) {

  if (i >= height || j >= width) {
    std::cerr << "Error: Index out of bounds!" << std::endl;
    return false;
  }

  data[i * width + j] = value;
  return true;
}

uint32_t Image::getWidth() { return width; }
uint32_t Image::getHeight() { return height; }
uint64_t Image::getTimestamp() { return header.timestamp; }
std::string Image::getSensorName() { return header.sensor_name; }

Image::~Image() {}

std::vector<uint8_t> Image::serialize() {
  // format :
  // size,timestamp,len(sensor_name),sensor_name,width,height,data

  std::vector<uint8_t> bytestream;

  for (int i = 0; i < sizeof(bytestream.size()); i++) {
    bytestream.push_back(0);
  }

  uint8_t timestamp_bytes[sizeof(header.timestamp)];
  *(typeof(header.timestamp) *)timestamp_bytes = header.timestamp;
  for (int i = 0; i < sizeof(header.timestamp); i++) {
    bytestream.push_back(timestamp_bytes[i]);
  }

  typeof(header.sensor_name.length())
      sensor_name_len_bytes[sizeof(header.sensor_name.length())];
  *(typeof(header.sensor_name.length()) *)sensor_name_len_bytes =
      header.sensor_name.length();
  for (int i = 0; i < sizeof(header.sensor_name.length()); i++) {
    bytestream.push_back(sensor_name_len_bytes[i]);
  }

  for (int i = 0; i < header.sensor_name.length(); i++) {
    bytestream.push_back(header.sensor_name[i]);
  }

  uint8_t width_bytes[sizeof(width)];
  *(typeof(width) *)width_bytes = width;
  for (int i = 0; i < sizeof(width); i++) {
    bytestream.push_back(width_bytes[i]);
  }

  uint8_t height_bytes[sizeof(height)];
  *(typeof(height) *)height_bytes = height;
  for (int i = 0; i < sizeof(height); i++) {
    bytestream.push_back(height_bytes[i]);
  }

  for (int i = 0; i < data.size(); i++) {
    bytestream.push_back(data[i]);
  }

  uint8_t size_bytes[sizeof(bytestream.size())];
  *(typeof(bytestream.size()) *)size_bytes = bytestream.size();
  for (int i = 0; i < sizeof(bytestream.size()); i++) {
    bytestream[i] = size_bytes[i];
  }

  return bytestream;
}

bool Image::deserialize(const std::vector<uint8_t> bytestream) {
  // format :
  // size,timestamp,len(sensor_name),sensor_name,width,height,data

  header.offset_position = 0;

  uint8_t size_bytes[sizeof(bytestream.size())];
  for (int i = 0; i < sizeof(bytestream.size());
       i++, header.offset_position++) {
    size_bytes[i] = bytestream[i];
  }
  typeof(bytestream.size()) size = *(typeof(bytestream.size()) *)size_bytes;
  if (size != bytestream.size()) {
    return false;
  }

  uint8_t timestamp_bytes[sizeof(header.timestamp)];
  for (int i = 0; i < sizeof(header.timestamp); i++, header.offset_position++) {
    timestamp_bytes[i] = bytestream[header.offset_position];
  }
  header.timestamp = *(typeof(header.timestamp) *)timestamp_bytes;

  typeof(header.sensor_name.length())
      sensor_name_len_bytes[sizeof(header.sensor_name.length())];
  for (int i = 0; i < sizeof(header.sensor_name.length());
       i++, header.offset_position++) {
    sensor_name_len_bytes[i] = bytestream[header.offset_position];
  }
  typeof(header.sensor_name.length()) sensor_name_len =
      *(typeof(header.sensor_name.length()) *)sensor_name_len_bytes;

  // We need this resize because std::string does not
  // automatically resize when you assign values to
  // individual indices
  header.sensor_name.resize(sensor_name_len);
  for (int i = 0; i < sensor_name_len; i++, header.offset_position++) {
    header.sensor_name[i] = bytestream[header.offset_position];
  }

  uint8_t width_bytes[sizeof(width)];
  for (int i = 0; i < sizeof(width); i++, header.offset_position++) {
    width_bytes[i] = bytestream[header.offset_position];
  }
  width = *(typeof(width) *)width_bytes;

  uint8_t height_bytes[sizeof(height)];
  for (int i = 0; i < sizeof(height); i++, header.offset_position++) {
    height_bytes[i] = bytestream[header.offset_position];
  }
  height = *(typeof(height) *)height_bytes;

  for (int i = 0; i < width * height; i++, header.offset_position++) {
    data[i] = bytestream[header.offset_position];
  }

  return true;
}

// class Image {
// public :
//     static Header lazyload(const std::vector<uint8_t> bytestream) ;
//     ~Image() ;
// } ;
// #endif
