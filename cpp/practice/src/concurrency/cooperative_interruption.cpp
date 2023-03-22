
#include "cooperative_interruption.h"

#include <iostream>
#include <stdint.h>
#include <thread>

namespace concurrency
{

void NumberPrinter::print_even_numbers(std::stop_token stop_token)
{
    uint64_t even_number = 2;
    while(!stop_token.stop_requested())
    {
        std::cout << even_number << std::endl;
        even_number += 2;
        std::this_thread::yield();
    }
}

void NumberPrinter::print_odd_numbers(std::stop_token stop_token, std::stop_source stop_source)
{
    uint64_t odd_number = 1;
    while(!stop_token.stop_requested())
    {
        std::cout << odd_number << std::endl;
        odd_number += 2;
        std::this_thread::yield();
    }
    stop_source.request_stop();
}

void NumberPrinter::main() const
{
    /*
	std::jthread jthread1(print_even_numbers);
	std::jthread jthread2(print_odd_numbers, jthread1.get_stop_source());

	std::stop_callback stop_callback1(jthread1.get_stop_token(), [] {
		std::cout << "Stop callback executed by thread1" << std::endl;
	});
	std::stop_callback stop_callback2(jthread2.get_stop_token(), [] {
		std::cout << "Stop callback executed by thread2" << std::endl;
	});

	jthread1.detach();
	jthread2.detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(2));

	jthread2.request_stop();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	*/
}

} // namespace concurrency
