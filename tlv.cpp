#include "tlv.h"


#define MAX_LEN   1048
#define HEAD_LEN   sizeof(messageHead)

void Tlv::ParseConcrete(const char* data, int data_len) {
  int type = 0;
  int length = 0;
  const char* buffer = data;
  while (true) {
	  if (data_len >= HEAD_LEN) {
		  type = *((int*)buffer);
		  length = *((int*)buffer + 1);
		  if (length <= 0 || length > MAX_LEN) {
			  printf("%s receive error packet! len %d\n", __func__, length);
			  break;
		  }
		  printf("%s type %d len %d\n", __func__, type, length);

		  if (length == data_len - HEAD_LEN) {
			  auto msg = new_message(type, length, data);
			  queue_.push(msg);
			  break;
		  }
		  else if (length > data_len - (int)HEAD_LEN) { //body is not enough
			  remain_data_.append(data, data_len);
			  break;
		  }
		  else {
			  auto msg = new_message(type, length, data);
			  queue_.push(msg);
			  buffer += length + HEAD_LEN;
			  continue;
		  }
	  }
	  else { //Not enough head information
		  remain_data_.append(data, data_len);
		  break;
	  }
  }
}


void Tlv::Parse(const char* data, int data_len) {
  if (!data || data_len > MAX_LEN || data_len <= 0) 
    return;

	if (remain_data_.empty()) {
		ParseConcrete(data, data_len);
	} else { 
		remain_data_.append(data, data_len);
		ParseConcrete(remain_data_.c_str(), remain_data_.length());
	}      
  
}

