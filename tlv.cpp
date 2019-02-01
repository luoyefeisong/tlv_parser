#include "tlv.h"

#define HEAD_LEN   sizeof(messageHead)

void Tlv::ParseConcrete(const char* data, int data_len) {
  int type = 0;
  int length = 0;

  if (data_len >= HEAD_LEN) {
    type = *((int*)data);
    length = *((int*)data + 1);

    if (length == data_len - HEAD_LEN) { 
      auto msg = new_message(type, length, data);
      queue_.push(msg);
    }
    else if (length > data_len - (int)HEAD_LEN) { //body is not enough
      next_data_.append(data, data_len);
      next_need_ = NEED_BODY;
    }
    else {  //body is enough
      auto msg = new_message(type, length, data);
      queue_.push(msg);
      remain_data_.append(data + length + HEAD_LEN, 
                          data_len - length - HEAD_LEN );
    }
  }
  else { //Not enough head information
    next_data_.append(data, data_len);
    next_need_ = NEED_HEAD;
  }
}

void Tlv::ParseConcreteNext2(const char* data, int data_len) {
  int type = *((int*)data);
  int length = *((int*)data + 1);

  if (length == data_len - HEAD_LEN) {
    auto msg = new_message(type, length, data + HEAD_LEN);
    queue_.push(msg);
    next_data_.clear();
    next_need_ = NEED_NONE;
  } else if (length > data_len - HEAD_LEN) {
    next_need_ = NEED_BODY;
  } else {
    auto msg = new_message(type, length, data + HEAD_LEN);
    queue_.push(msg);
    remain_data_.append(data + length + HEAD_LEN, 
                        data_len - length - HEAD_LEN );
    next_data_.clear();
    next_need_ = NEED_NONE;
  }   
}

void Tlv::ParseConcreteNext(const char* data, int data_len) {
  if (next_need_ == NEED_HEAD && data_len < HEAD_LEN) 
    next_need_ = NEED_HEAD;
  else 
    ParseConcreteNext2(data, data_len);
}

void Tlv::ParseConcreteRemain(const char* data, int data_len) {
  int type = 0;
  int length = 0;

  if (data_len >= HEAD_LEN) {
    type = *((int*)data);
    length = *((int*)data + 1);

    if (length == data_len - HEAD_LEN) { 
      auto msg = new_message(type, length, data);
      queue_.push(msg);
      remain_data_.clear();
    }
    else if (length > data_len - (int)HEAD_LEN) { //body is not enough
      next_data_.append(data, data_len);
      next_need_ = NEED_BODY;
      remain_data_.clear();
    }
    else {  //body is enough
      auto msg = new_message(type, length, data);
      queue_.push(msg);
      remain_data_.erase(0, length + HEAD_LEN );
    }
  }
  else { //Not enough head information
    next_data_.append(data, data_len);
    next_need_ = NEED_HEAD;
  }
}

void Tlv::Parse(const char* data, int data_len) {
  if (!data || data_len > MAX_LEN || data_len <= 0) 
    return;
  int i = 1;
  while(i || !remain_data_.empty()) {
    i = 0;
    if (!remain_data_.empty()) {
      ParseConcreteRemain(remain_data_.c_str(), remain_data_.size());
    } else {
      if (next_need_ == NEED_NONE) {
        ParseConcrete(data, data_len);
      } else {
        next_data_.append(data, data_len);
        ParseConcreteNext(next_data_.c_str(), next_data_.size());
      }
    } 
  }
}

