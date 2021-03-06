#pragma once

#include <vector>

#include <memory>


#pragma warning(disable : 4200)  


struct messageHead
{
public:
	int type;
	int data_length;
	char data[0];
};

typedef messageHead message;

std::shared_ptr<messageHead> new_head(int type = 0, int data_length = 0);

std::shared_ptr<message> new_message(std::shared_ptr<messageHead> head);

std::shared_ptr<message> new_message(int type, int data_length = 0, const void *data = NULL);

inline int length_of_message(std::shared_ptr<messageHead> msg) {
	return sizeof(messageHead) + msg->data_length;
}
