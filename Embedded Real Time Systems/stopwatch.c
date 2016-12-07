#include <sys/prex.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>


int toggle_flag;

/* clock variables */
int sec, min, hour;
int add_hours, add_min, sub_sec;

/* Stopwatch Variables */
int start_flag, pause_flag, reset_flag, stop_flag, stop_flag_lock;
int stop_time, stopped_time, t;
int stop_A, stop_B;
int tic=0, tac=0;

int t1 = 0, t2 = 0 ;

/* timeval struct to get the current time for the clock*/
struct __timeval getTime(void) {
	device_t rtc_dev;
	struct __timeval time;

	device_open("rtc", 0, &rtc_dev);
	device_ioctl(rtc_dev, RTCIOC_GET_TIME, &time);
	device_close(rtc_dev);
	return time;
}

/* clock implementation */
void displayClock(void) {

	sec  = (getTime().tv_sec % 60) - sub_sec;
	min  = (add_min + getTime().tv_sec / 60) % 60;
	hour = (add_hours + getTime().tv_sec / 3600) % 24;
	printf("\r%02d:%02d:%02d    ", hour, min, sec);
}

/* stopwatch implementation */
void displayStopwatch(void){


	/* this is for the stopped-time passed after pressing the 'S' button */
	if (stop_flag && stop_flag_lock){
		stop_flag_lock = 0;
		sys_time(&stop_A);
	}
	if (!stop_flag && !stop_flag_lock){
		stop_flag_lock = 1;
		sys_time(&stop_B);
		stopped_time += stop_B-stop_A;
	}

	/* reset the timers after pressing the 'R' button */
	if (reset_flag){
		sys_time(&tic);
		sys_time(&tac);
		reset_flag = !reset_flag;
		stop_flag_lock = 1;
		t = stopped_time = 0;
	}

	/* get the sys_time if the stopwatch is not stopped */
	if (!stop_flag)
		sys_time(&tac);

	/* sub the stopped-time except is the stopwatch is paused */
	if(!pause_flag)
		t = tac - tic - stopped_time;

	/* the obvious.. print stopwatch's time */ 	
	printf("\r%02d:%02d:%02d.%02d",	((t)/3600000)%24,
								 	((t)/60000)%60, 
									((t)/1000)%60, 
									((t)/10)%100);

}

/* setting the mode, tcsetattr and tcgetattr functions
 * to read chars without the need of pressing the 
 * enter button after pressing every button 
 */
void mode(int f)
{
	static struct termios oldt, newt;

	if ( f == 1 ) {
		tcgetattr( STDIN_FILENO, &oldt);
		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );
		tcsetattr( STDIN_FILENO, TCSANOW, &newt);
	}
	else
		tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}




int tcsetattr(int fd, int opt, const struct termios *t)
{
	struct termios localterm;
	device_t ttyDevice;
	int returnedValue; 	
	if (opt & TCSASOFT) {
		localterm = *t;
		localterm.c_cflag |= CIGNORE;
		t = &localterm;
	}
	switch (opt & ~TCSASOFT) {
	case TCSANOW:
			device_open("tty", 2, &ttyDevice);
			returnedValue = device_ioctl( ttyDevice, TIOCSETA, t);
			device_close(ttyDevice);
			return  returnedValue;			
			break;
					
	case TCSADRAIN:
			device_open("tty", 2, &ttyDevice);
			returnedValue = device_ioctl( ttyDevice, TIOCSETAW, t);
			device_close(ttyDevice);
			return  returnedValue;				
			break;
				
	case TCSAFLUSH:
			device_open("tty", 2, &ttyDevice);
			returnedValue = device_ioctl( ttyDevice, TIOCSETAF, t);
			device_close(ttyDevice);
			return  returnedValue;	
			break;
				
	default:
		errno = EINVAL;
		return (-1);
	}
}
 
int tcgetattr(int fd, struct termios *t) {
	device_t ttyDevice;
	int returnedValue;
	device_open("tty", 2, &ttyDevice);
	returnedValue = device_ioctl( ttyDevice, TIOCGETA, t);
	device_close(ttyDevice);
	return  returnedValue;
}

/* main function */
int main(void)
{
	/* initialize */
	device_t ttyDevice;
	char button;
	int input;
	toggle_flag = 0;
	reset_flag = 1;
	pause_flag = 0;
	stop_flag = 1;
	stop_flag_lock = 1;
	add_hours = 0;
	add_min = 0;
	sub_sec = 0;
	stopped_time = 0;
	mode(1);


	/* entering loop */
	while(1){
		device_open( "tty", 0, &ttyDevice );
		device_ioctl( ttyDevice, TIOCINQ, &input);
		device_close( ttyDevice);

		
		if( input > 0 ){
			button = getchar();
			switch(button){
				case 't':
					toggle_flag=!toggle_flag;	
					break;
				case 'r':	
					reset_flag = !reset_flag;
					tic = tac = stopped_time = 0 ;
					break;
				case 'p':
					if (!stop_flag)
						pause_flag = !pause_flag;
					break;
				case 's':
					if (!pause_flag)
						stop_flag = !stop_flag;
					break;
				case 'h':
					add_hours++;
					break;
				case 'm':
					add_min++;
					break;
				case 'z':
					sub_sec = getTime().tv_sec %60;
					break; 

			}
		}
		if (!toggle_flag){
			displayClock();
		}
		else if (toggle_flag)
			displayStopwatch();	

	timer_sleep(10,0);
	}
	mode(0); /* for general use - not needed in this implementation */
}
