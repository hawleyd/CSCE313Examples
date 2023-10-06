#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/select.h>      /* added by wje */
#include <time.h>            /* added by wje */

volatile long long count_major_loop      =0;
volatile long long count_signals_caught  =0;
volatile long long count_characters_input=0;
volatile int       flag_quit             =0;

size_t
length(const char *something)
{
        size_t result=0;

        while(*something++) {
                result++;
        }

        return result;
}

void
sig_io(int signo)
{
        ssize_t        read_count;

        char           c_array[4];

        fd_set         read_set;

        struct timeval timeout_value;

        count_signals_caught++;

        FD_ZERO(&read_set);

        FD_SET(STDIN_FILENO,&read_set);

        timeout_value.tv_sec =0;
        timeout_value.tv_usec=0;

        if(select(STDIN_FILENO+1,&read_set,NULL,NULL,&timeout_value)>0)
        {
                read_count=read(STDIN_FILENO,c_array,1);

                if(read_count < 0) {
                        write(STDERR_FILENO,"got error\r\n",
                                     length("got error\r\n"));
                        flag_quit=1;
                }
                else
                if(read_count==0)
                {
                        write(STDERR_FILENO,"got eof\r\n",
                                     length("got eof\r\n"));
                        flag_quit=1;
                }
                else
                if (c_array[0] == 'q') {
                        write(STDERR_FILENO,"got quit request\r\n",
                                     length("got quit request\r\n"));
                        flag_quit=1;
                }
                else
                {
                        count_characters_input++;

                        write(STDOUT_FILENO,"read character ",
                                     length("read character "));

                        write(STDOUT_FILENO,c_array,1);

                        write(STDOUT_FILENO,"\r\n",
                                     length("\r\n"));
                }
        }
}

int
main(int argc, char **argv)
{
        time_t           tick_tick;

        struct sigaction sa;

        memset(&sa, 0, sizeof(sa));
        sa.sa_handler = sig_io;

        if (sigaction(SIGIO, &sa, NULL) < 0) {
                fprintf(stderr,"[ERROR]: sigaction error\n");
                exit(1);
        }

        if (fcntl(0, F_SETOWN, getpid()) < 0) {
                fprintf(stderr,"[ERROR]: fcntl F_SETOWN error\n");
                exit(1);
        }

        if (fcntl(0, F_SETFL, O_NONBLOCK|O_ASYNC) < 0) {
                fprintf(stderr,"[ERROR]: fcntl error for O_NONBLOCK|O_ASYNC\n");
                exit(1);
        }

        tick_tick=time(NULL);

        while (!flag_quit) {
                if(tick_tick!=time(NULL)) {
                        tick_tick=time(NULL);
                        printf("1");
                        fflush(stdout);
                }
                count_major_loop++;
        }

        printf("major loop count: %lld\r\n",count_major_loop      );
        printf("signals caught  : %lld\r\n",count_signals_caught  );
        printf("characters input: %lld\r\n",count_characters_input);

        return 0;
}