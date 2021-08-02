
/*!*****************************************************************************
\file ProducerConsumer.hpp
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

#ifndef _PRODUCER_CONSUMER_HPP_
#define _PRODUCER_CONSUMER_HPP_

#include <iostream>
#include <optional>
#include <functional>

#include "GoBackN_RDT.h"



 //TaskQueue Constructor
template <typename TItem/*Type of item in the buffer*/, 
	typename TAction/*Type of actionFunction to execute on the item*/, 
	typename TOnDisconnect/*Disconnect/shutdown the ListenerSocket*/>
TaskQueue<TItem, TAction, TOnDisconnect>::TaskQueue(
	size_t workerCount			/*number of threads working*/, 
	size_t slotCount			/*number of slots in the buffer*/, 
	TAction action				/*ActionFunction to execute on the item*/, 
	TOnDisconnect onDisconnect	/*Winsock DisconnectFunction*/
) :
	_slotCount{ slotCount },
	_itemCount{ 0 },
	_onDisconnect{ onDisconnect },
	_stay{ true } //boolean for the every thread to (true)continue working or (false)stop working
{
	//Start Execution of all the number of threads
	for (size_t i = 0; i < workerCount; ++i)
	{
		//Each thread - executes the work() function
		//work() function parameters - 1.this TaskQueue, 2.ActionFunction to execute on an item
		_workers.emplace_back(&work, std::ref(*this), std::ref(action));
	}
}


//function for main thread to produce an item into the buffer
template <typename TItem, typename TAction, typename TOnDisconnect>
void TaskQueue<TItem, TAction, TOnDisconnect>::produce(
	TItem& item/*item to put in the buffer*/)
{
	// Non-RAII unique_lock to be blocked by a producer who needs a slot.
	{
		// Wait for an available slot...
		std::unique_lock<std::mutex> slotCountLock{ _slotCountMutex };
		//condition_variable to check the number of slotCount
		//Main Thread to continue waiting if the slotCout is 0, no space to put this item
		//releases mutex unique_lock if the labda returns false
		_producers.wait(slotCountLock, [&]() { return _slotCount > 0; });
		//Decreases slot count as the main thread is putting an item into the buffer
		--_slotCount;
	}
	// RAII lock_guard locked for buffer.
	{
		// Lock the buffer.
		std::lock_guard<std::mutex> bufferLock{ _bufferMutex };
		//insert item into the buffer
		_buffer.push(item);
	}
	// RAII lock_guard locked for itemCount.
	{
		// Announce available item.
		std::lock_guard<std::mutex> itemCountLock(_itemCountMutex);
		//increase itemCount
		++_itemCount;
		//notify a consumer condition_variable waiting for itemCount to increase
		//wakes a thread up
		_consumers.notify_one();
	}
}


//called by work() function in each thread
//Each thread can consume an item from the buffer in this taskQueue
template <typename TItem, typename TAction, typename TOnDisconnect>
std::optional<TItem> TaskQueue<TItem, TAction, TOnDisconnect>::consume()
{
	std::optional<TItem> result = std::nullopt;
	// Non-RAII unique_lock to be blocked by a consumer who needs an item.
	{
		// Wait for an available item or termination...
		std::unique_lock<std::mutex> itemCountLock{ _itemCountMutex };
		//condition_variable to check the number of slotCount
		//This Thread to continue waiting if the _itemCount is 0, no items in the taskQueue buffer
		//releases mutex unique_lock if the lambda returns false
		_consumers.wait(itemCountLock, [&]() { return (_itemCount > 0) || (!_stay); });

		//if the condition variable returns true for !_stay
		//and the itemcount will be 0
		if (_itemCount == 0)
		{
			//notify another thread to wake up
			_consumers.notify_one();
			//return null item
			return result;
		}
		//decrease number of itemCount in the buffer
		--_itemCount;
	}
	// RAII lock_guard locked for buffer.
	{
		// Lock the buffer.
		std::lock_guard<std::mutex> bufferLock{ _bufferMutex };

		result = _buffer.front();
		_buffer.pop();
	}
	// RAII lock_guard locked for slots.
	{
		// Announce available slot.
		std::lock_guard<std::mutex> slotCountLock{ _slotCountMutex };
		++_slotCount;
		//notify a _producers condition_variable waiting for _slotCount to increase
		//wakes the main thread up
		_producers.notify_one();
	}

	//returns the item consumed from the buffer
	return result;
}


//Each thread - executes the work() function
template <typename TItem, typename TAction, typename TOnDisconnect>
void TaskQueue<TItem, TAction, TOnDisconnect>::work(
	TaskQueue<TItem, TAction, TOnDisconnect>& tq,//the TaskQueue
	TAction& action/*ActionFunction to execute on the item*/)
{
	//while True loop 
	//   to continue to take on another item
	//	 after one item is consumed
	while (true)
	{
		//std::cout
		//	<< "Thread ["
		//	<< std::this_thread::get_id()
		//	<< "] is waiting for a task."
		//	<< std::endl;

		//Thread consumes an item from the TaskQueue buffer
		std::optional<TItem> item = tq.consume();

		//if the item is returned as null - no more items to be consumed
		// && also 
		//		some client has changed the _stay to false
		//		hence asking all the threads to terminnate
		//		hence asking the server to terminate
		if (!item->execute())
		{
			// Termination of idle threads.
			break;
		}

		//std::cout
		//	<< "Thread ["
		//	<< std::this_thread::get_id()
		//	<< "] is executing a task."
		//	<< std::endl;

		//Execute the actionFunction on the item consumed from TaskQueue buffer
		//If the actionFunction returns false
		//		some client has changed the _stay to false
		//		hence asking all the threads to terminate
		//		hence asking the server to terminate


		//(*this.*pt2ConstMember)(12, 'a', 'b');


		int bii = 0;
		int bii1 = 0;
		int bii2 = 0;
		if (!item->execute())
		{
			// Decision to terminate workers.
			// calls function to Shutdown and close listenerSocket
			//std::cout << "disconnect" << std::endl;
			tq.disconnect();
			tq._consumers.notify_all();
		}


	}



	//std::cout
	//	<< "Thread ["
	//	<< std::this_thread::get_id()
	//	<< "] is exiting."
	//	<< std::endl;
}


//called by work() function in each thread
//TaskQueue disconnect function to call winsock disconnectFunction
template <typename TItem, typename TAction, typename TOnDisconnect>
void TaskQueue<TItem, TAction, TOnDisconnect>::disconnect()
{
	_stay = false;
}


//TaskQueue Destructor
//waits for all threads to join() before exiting main Thread
template <typename TItem, typename TAction, typename TOnDisconnect>
TaskQueue<TItem, TAction, TOnDisconnect>::~TaskQueue()
{
	// Shutdown and close listenerSocket
	disconnect();
	for (std::thread& worker : _workers)
	{
		worker.join();
	}
}

#endif