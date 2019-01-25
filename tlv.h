#pragma once
#include <string>
#include <queue>
#include <memory>
#include "message.h"


class Ice;
class Tlv {
 public:
	Tlv() = default;;
  ~Tlv() = default;
  void ParseConcrete(const char* data, int data_len);
  void Parse(const char* data, int data_len);

 private:
  std::string remain_data_;  //the data to be handled next time 
  std::string todo_data_;  //the data to be handled this time 
 public:
  std::queue<std::shared_ptr<message>> queue_;
};

