#pragma once
#include <iostream>			// cout, cerr


#define MESSAGE_MAX_BUFEFR 1100

class Message
{
public:
    size_t size_{0};
    char * message;

    //Default Constructor
    Message()
    {
        message = new char[MESSAGE_MAX_BUFEFR];
    }

    //Custom Constructor
    Message(const char* mess, const size_t& size)
        :size_{ size }
    {
        message = new char[MESSAGE_MAX_BUFEFR];
        memcpy(message, mess, size);
    }

    //Copy Constructor
    Message(const Message& copy)
        : size_{ copy.size_ }, message{}
    {
        //message = new char[size];
        message = new char[MESSAGE_MAX_BUFEFR];
        memcpy(message, copy.message, size_);
    }

    //Copy Constructor
    Message(Message&& copy) noexcept
        :size_{ copy.size_ }
    {
        //message = new char[size];
        message = new char[MESSAGE_MAX_BUFEFR];
        memcpy(message, copy.message, size_);
    }

    //Copy Assignment
    void operator=(const Message& copy)
    {
        size_ = { copy.size_ };
        //message = new char[size];
        memcpy(message, copy.message, size_);
    }


    // Destructor
    ~Message()
    {
        //delete message;
        delete[] message;
    }


};
