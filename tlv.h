#pragma once
#include <string>
#include <queue>
#include <memory>
#include "message.h"

typedef enum need {
  NEED_NONE = 0,
  NEED_HEAD = 1,
  NEED_BODY = 2
}need_e;

class Ice;
class Tlv {
 public:
	Tlv() :next_need_(NEED_NONE){};
  ~Tlv() = default;
  void ParseConcrete(const char* data, int data_len);
  void ParseConcreteRemain(const char* data, int data_len);
  void ParseConcreteNext(const char* data, int data_len);
  void ParseConcreteNext2(const char* data, int data_len);
  void Parse(const char* data, int data_len);

 private:
  std::string remain_data_;  //the data to be handled this time 
  std::string next_data_;   //the data to be handled next time
  need_e next_need_;  
 public:
  std::queue<std::shared_ptr<message>> queue_;
};

