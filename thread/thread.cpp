//
// Created by star on 18-3-7.
//
#include <boost/thread.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

void wait(int seconds)
{
    boost::this_thread::sleep(boost::posix_time::seconds(seconds));
}

void thread()
{
    for (int i = 0; i < 5; ++i)
    {
        wait(1);
        std::cout << i << " ";
        std::cout << "current time : "<< boost::gregorian::day_clock::local_day()<<std::endl;
    }
}

int main()
{
    time_t time1;
    boost::thread t(thread);
    t.join();
}
