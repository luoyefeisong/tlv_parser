#include "message.h"
#include<iostream>
#include <cstring>



void deleteMessage(messageHead* item)
{
	free(item);
}

std::shared_ptr<messageHead> new_head(int type, int data_length) {
	messageHead * head = (messageHead *)malloc(sizeof(messageHead));
	head->type = type;
	head->data_length = data_length;
	return std::shared_ptr<messageHead>(head, deleteMessage);
}

std::shared_ptr<message> new_message(std::shared_ptr<messageHead> head) {

	return new_message(head->type, head->data_length);
}

std::shared_ptr<message> new_message(int type, int data_length, const void *data) {
	message * msg = (message *)malloc(sizeof(messageHead) + data_length);
	msg->type = type;
	msg->data_length = data_length;
	if (data && data_length)
		memcpy(msg->data, data, data_length);

	return std::shared_ptr<message>(msg, deleteMessage);
}

