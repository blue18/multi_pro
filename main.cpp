// July 31, 2018

#include <iostream>
#include <thread>
#include <cstdlib>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

// Mutex for critical section
std::mutex theLock;

// Condition variable 
std::condition_variable cond_var;
bool ready = false;

// Shared queue for threads 
std::queue<int> Q;
int sumOne;
int sumTwo;

// Produces random values
void producer() {
    int n = 0;
    int numOfRanNum = 10;
    srand(time(NULL));

    // Getting the lock on Q using mutex 
    std::unique_lock<std::mutex> locker(theLock);

    // Creating and storing random num in Q
    for(int i = 0; i < numOfRanNum; i++) {
	n = rand() % 100;	
	Q.push(n);
    }	    

    std::cout << "Producer is done. Nofity all threads." << std::endl;
    ready = true;
    locker.unlock();
    cond_var.notify_all();
}

void consumerOne() {
    while(true) {
	// Wait for producer to store values in Q
	std::unique_lock<std::mutex> locker(theLock);
	cond_var.wait(locker, []{ return ready; });

	// after the wait, we own the lock
	// Take a value from Q and add it to the total sum
	if(Q.size() > 0) {
	    sumOne += Q.front();
	    Q.pop();
	} else {
	    // if Q is empty, break from loop
	    locker.unlock();
	    break;
	}

	locker.unlock();
    }
}

void consumerTwo() {
    while(true) {
	// Wait for producer to store values in Q
	std::unique_lock<std::mutex> locker(theLock);
	cond_var.wait(locker, []{ return ready; });

	// after the wait, we own the lock
	// Take a value from Q and add it to the total sum
	if(Q.size() > 0) {
	    sumTwo += Q.front();
	    Q.pop();
	} else {
	    // if Q is empty, break from loop
	    locker.unlock();
	    break;
	}

	locker.unlock();
    }
}

int main() {

    // Clean initil console 
    system("clear");

    // This thead is launched
    std::thread producer_thread(producer);
    std::thread consumerOne_thread(consumerOne);
    std::thread consumerTwo_thread(consumerTwo);


    // Wait for thread
    if(producer_thread.joinable()) {
	producer_thread.join();
    }
    
    if(consumerOne_thread.joinable()) {
	consumerOne_thread.join();
    }

    if(consumerTwo_thread.joinable()) {
	consumerTwo_thread.join();
    }

    std::cout << "Player One " << sumOne << std::endl;
    std::cout << "Player Two " << sumTwo << std::endl;

    return 0;
}
