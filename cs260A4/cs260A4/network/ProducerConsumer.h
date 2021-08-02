
/*!*****************************************************************************
\file ProducerConsumer.h
\author Luo Yu Xuan
\par DP email: yuxuan.luo\@digipen.edu
\par Course: CS260
\par Section: A
\par Programming Assignment 2
\date 06-19-2021
\brief
This file contains the class declarations and definitions
for TaskQueue.
*******************************************************************************/
/*******************************************************************************
 * A producer-consumer pattern for the multi-threaded execution
 ******************************************************************************/

#ifndef _PRODUCER_CONSUMER_H_
#define _PRODUCER_CONSUMER_H_

#include <vector>
#include <queue>
#include <mutex>
#include <optional>

#include "GoBackN_RDT.h"


template <typename TItem, typename TAction, typename TOnDisconnect>
class TaskQueue
{
public:
	//TaskQueue Constructor
	TaskQueue(size_t workerCount, size_t slotCount, TAction action, 
		TOnDisconnect disconnect);
	//TaskQueue Destructor
	~TaskQueue();

	//called by work() function in each thread
	//Each thread can consume an item from the buffer in this taskQueue
	std::optional<TItem> consume();

	//function for main thread to produce an item into the buffer
	void produce(TItem& item);

	//Deletion of default constructor, 
	//			copy constructor
	// 			move constructor
	// 			Assignment Operator
	// 		    Assignment Move Operator
	TaskQueue() = delete;
	TaskQueue(const TaskQueue&) = delete;
	TaskQueue(TaskQueue&&) = delete;
	TaskQueue& operator=(const TaskQueue&) = delete;
	TaskQueue& operator=(TaskQueue&&) = delete;

private:

	//Each thread - executes the work() function
	static void work(TaskQueue<TItem, TAction, TOnDisconnect>& tq, TAction& action);
	
	//called by work() function in each thread
	//TaskQueue disconnect function to call winsock disconnectFunction
	void disconnect();

	// Pool of worker threads.
	std::vector<std::thread> _workers;
	std::vector<std::thread> _CheckQuitStatus;

	// Buffer of slots for items.
	std::mutex _bufferMutex;
	std::queue<TItem> _buffer; 	//queue of items in the buffer


	// Count of available slots.
	std::mutex _slotCountMutex;
	size_t _slotCount;
	// Critical section condition for decreasing slots.
	std::condition_variable _producers;


	// Count of available items.
	std::mutex _itemCountMutex;
	size_t _itemCount;
	// Critical section condition for decreasing items.
	std::condition_variable _consumers;

	volatile bool _stay; //boolean for the every thread to (true)continue working or (false)stop working

	TOnDisconnect& _onDisconnect; /*Winsock DisconnectFunction*/



};

#include "ProducerConsumer.hpp"

#endif