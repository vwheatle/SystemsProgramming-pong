#include <stdlib.h>

#include <signal.h>
#include <sys/time.h>

/*
	set_ticker( number_of_milliseconds )
		 arranges for the interval timer to issue
		 SIGALRM's at regular intervals
	returns -1 on error, 0 for ok
	arg in milliseconds, converted into microseconds
*/

int set_ticker(int n_msecs) {
	long n_sec = n_msecs / 1000;
	long n_usecs = (n_msecs % 1000) * 1000L;

	struct itimerval time;
	time.it_interval.tv_sec = n_sec;    // set reload
	time.it_interval.tv_usec = n_usecs; // new ticker value
	time.it_value.tv_sec = n_sec;       // store this
	time.it_value.tv_usec = n_usecs;    // and this

	return setitimer(ITIMER_REAL, &time, NULL);
}
