#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
using namespace std;

/* structure for looking up IP address 
struct addrinfo{
int ai_flags;
int ai_family; // AF_INET=IPv4,AF INET6= IPv6
int ai_socktype; // TCP or UDP
int ai_protocol;
socklen_t ai_addrlen; // length of ai_addr
struct sockaddr* ai_addr; // contains IP+PORT
char* ai_canonname; // canonical name
struct addrinfo* ai_next; // next pointer of result link list
};*/

/* data structure for IP details (+PORT) 
struct sockaddr_in{
short sin_family; // IPv4 or IPv�6
unsigned short sin_port; // port number
struct in_addr sin_addr; // 32 bit IP
char sin_zero [8];
}; */

/* just a wrapper for the numeric IP 
struct in_addr{
    unsigned long s_addr;
};*/

int getAddr(char *addrName)
{

    int status;
    int numAddresses = 0;
    struct addrinfo hints;
    struct addrinfo *servinfo; // will point to the results

    // preparing hints data structure
    memset (&hints, 0, sizeof hints); // make sure the struct is empty
    hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets

    // look up the IP address from the name - returns 0 on success
    if(status = getaddrinfo(addrName, "80", &hints, &servinfo)==0)
    {
        addrinfo *p;
        for(p = servinfo; p != NULL; p = p->ai_next) {
        void *addr;
        char ipver[5];
        char ip4str[INET_ADDRSTRLEN];
        char ip6str[INET6_ADDRSTRLEN];

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            strcpy(ipver, "IPv4");
            if(inet_ntop(p->ai_family, addr, ip4str, sizeof(ip4str))!=NULL)
        {
            printf(" %s: %s\n", ipver, ip4str);
        }
        else
        {
            printf(" %s: %s", ipver, ip4str);
            printf(" - Error with inet_ntop\n");
        }
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            strcpy(ipver, "IPv6");
            if(inet_ntop(p->ai_family, addr, ip6str, sizeof(ip6str))!=NULL)
        {
            printf(" %s: %s\n", ipver, ip6str);
        }
        else
        {
            printf(" %s: %s", ipver, ip6str);
            printf(" - Error with inet_ntop\n");
        }
        }
        // convert the IP to a string and print it:
        
        numAddresses++;
        }
    }
    return numAddresses;

}

int main (int argc, char *argv[]) {
    char *address = NULL;	//ip address to look up
    int status;
    // read arguments
    int opt;
	while ((opt = getopt(argc, argv, "a:")) != -1) {
		switch (opt) {
			case 'a':
				address = optarg;
                break;
            default:
                printf("Usage: %s -a ip_address\n", argv[0]);
                exit(1);
		}
	}

    printf("Looking up %s\n", address);
    status = getAddr(address);
    printf("Returned %d address(es)\n", status);
    return 0;
}