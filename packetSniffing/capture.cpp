#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pcap/pcap.h>

//modeled after https://www.tcpdump.org/pcap.html
//and https://github.com/cahillone/Packet-Processor/blob/master/packets.c

void printMAC(const u_char *packet_data)
{
  // print MAC src address
  printf("%02X:", packet_data[6]);
  printf("%02X:", packet_data[7]);
  printf("%02X:", packet_data[8]);
  printf("%02X:", packet_data[9]);
  printf("%02X:", packet_data[10]);
  printf("%02X", packet_data[11]);
  printf(" -> ");
  // print MAC dst address
  printf("%02X:", packet_data[0]);
  printf("%02X:", packet_data[1]);
  printf("%02X:", packet_data[2]);
  printf("%02X:", packet_data[3]);
  printf("%02X:", packet_data[4]);
  printf("%02X ", packet_data[5]);
  return;
}
// input: packet data from PCAP
// return value: integer value containing the current packet's type/length value
//  example return values: 0x86dd, 0x0800
// description: checkType() looks at Byte 12 and 13 of packet_data (the
//  type/length field of packet header)
int checkType(const u_char *packet_data)
{
  uint16_t type;
  type = (packet_data[12] << 8) | packet_data[13];
  return type;
}


// input: packet data from PCAP
// return value: printIPv4info() returns 0 on success, returns -1 on error
// description: printIPv4info() cleanly prints IPv4 source and destination addresses
int printIPv4info(const u_char *packet_data)
{
  printf("[IPv4] ");

  // print IPv4 source address
  char src[INET_ADDRSTRLEN];
  char dst[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, ((const void *)(packet_data+26)), src, INET_ADDRSTRLEN);
  if (src == NULL)
  {
    printf("error in inet_ntop ipv4 src\n");
    return -1;
  }
  printf("%s", src);
  printf(" -> ");

  // print IPv4 destination address
  inet_ntop(AF_INET, ((const void *)(packet_data+30)), dst, INET_ADDRSTRLEN);
  if (dst == NULL)
  {
    printf("error in inet_ntop ipv4 dst\n");
    return -1;
  }
  printf("%s", dst);
  return 0;
}

// return value: printARPinfo returns 0 on success, returns -1 on error
// description: printARPinfo first checks if request or reply
//  on requests the requester's IP is first printed followed by the requested IP
//  on replys the provided IP is printed followed by the Ethernet address mapping
int printARPinfo(const u_char *packet_data)
{
  printf("[ARP] ");
  
  char src[INET_ADDRSTRLEN];
  char dst[INET_ADDRSTRLEN];
  if (packet_data[21] == 0x01) // request
  {
    // print requester's IP
    inet_ntop(AF_INET, ((const void *)(packet_data+28)), src, INET_ADDRSTRLEN);
    if (src == NULL)
    {
      printf("error in inet_ntop arp request src\n");
      return -1;
    }

    // print requested IP
    inet_ntop(AF_INET, ((const void *)(packet_data+38)), dst, INET_ADDRSTRLEN);
    if (dst == NULL)
    {
      printf("error in inet_ntop arp request dst\n");
      return -1;
    }
    printf("%s requests %s", src, dst);
  }

  if (packet_data[21] == 0x02) // reply
  {
    // print provided IP
    inet_ntop(AF_INET, ((const void *)(packet_data+28)), src, INET_ADDRSTRLEN);
    if (src == NULL)
    {
      printf("error in inet_ntop arp reply src\n");
      return -1;
    }
    printf("%s at ", src);

    // print Ethernet address mapping
    printf("%02X:", packet_data[32]);
    printf("%02X:", packet_data[33]);
    printf("%02X:", packet_data[34]);
    printf("%02X:", packet_data[35]);
    printf("%02X:", packet_data[36]);
    printf("%02X", packet_data[37]);
  }
  return 0;
}

// input: packet data from PCAP
// return value: printIPv6info() returns 0/-1 on success/error
// description: printIPv6info() cleanly prints the source and destination IPv6 address
int printIPv6info(const u_char *packet_data)
{
  printf("[IPv6] ");

  // print IPv6 source address
  char src[INET6_ADDRSTRLEN];
  char dst[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, ((const void *)(packet_data+22)), src, INET6_ADDRSTRLEN);
  if (src == NULL)
  {
    printf("error in inet_ntop ipv4 src\n");
    return -1;
  }
  printf("%s", src);
  printf(" -> "); 

  // print IPv6 destination address
  inet_ntop(AF_INET6, ((const void *)(packet_data+38)), dst, INET6_ADDRSTRLEN);
  if (dst == NULL)
  {
    printf("error in inet_ntop ipv4 dst\n");
    return -1;
  }
  printf("%s", dst);
  return 0;
}

// input: packet data from PCAP
// return value: printVLANinfo() returns 0 on success
// description: printVLANinfo() prints the VLAN ID
int printVLANinfo(const u_char *packet_data)
{
  printf("[VLAN] ");
  int vlanID = 0;
  vlanID = packet_data[14] + packet_data[15];
  printf("ID = %d", vlanID);
  return 0;
}
void printHelp(char* arg)
{
  printf ("Usage: %s -p number of packets to capture\n", arg);
  exit(1);
}


int main(int argc,char *argv[])
{
    char error[PCAP_ERRBUF_SIZE];
    pcap_if_t *interfaces;
    pcap_if_t *temp;
    pcap_t* handle;
    struct bpf_program fp;
    char *dev;
    int i=0;
    int opt;                /*command line options*/
    char filter_exp[] = "portrange 8080-8082";
    
    bpf_u_int32 mask;		/* The netmask of our sniffing device */
    bpf_u_int32 net;		/* The IP of our sniffing device */
    struct pcap_pkthdr *header;	/* The header that pcap gives us */
	const u_char *packet;		/* The actual packet */
    int ret;                /*return values*/
    int errorCode;
    int packetType;         /*IP4 vs IP6*/
    int numPacketsToGrab;

    //take care of command line arguments first
    if (argc !=3)
    {

        printHelp(argv[0]);
    }
    else
    {
        while ((opt = getopt(argc, argv, "p:")) != -1) {
            switch (opt) {
                case 'p':
                numPacketsToGrab = atoi(optarg);
                break;
            default:
            printHelp(argv[0]);
            }
        }
   
        //got arguments, now do the work

        //get devices
        if(pcap_findalldevs(&interfaces,error)==-1)
        {
            printf("\nerror in pcap findall devs");
            return -1;   
        }
        //save first device
        dev = interfaces->name;

        //print devices
        printf("The interfaces present on the system are:\n");
        for(temp=interfaces;temp;temp=temp->next)
        {
            printf("%d  :  %s\n",i++,temp->name);
        }

        //get network info from self
        if (pcap_lookupnet(dev, &net, &mask, error) == -1) {
            fprintf(stderr, "Can't get netmask for device %s\n", dev);
            net = 0;
            mask = 0;
        }
        //open device
        printf("Device used will be: %s\n", dev);
        handle = pcap_open_live(dev, BUFSIZ, 1, 1000, error);
        if (handle == NULL) {
            fprintf(stderr, "Couldn't open device %s: %s\n", dev, error);
            return(2);
        }     

        if (pcap_compile(handle, &fp, filter_exp, 0, net) == -1) {
            fprintf(stderr, "Couldn't parse filter %s: %s\n", filter_exp, pcap_geterr(handle));
            return(2);
        }
        if (pcap_setfilter(handle, &fp) == -1) {
            fprintf(stderr, "Couldn't install filter %s: %s\n", filter_exp, pcap_geterr(handle));
            return(2);
        }

        /* get packets*/
        ret = pcap_next_ex(handle, &header, &packet);
        for (int i = 0; i< numPacketsToGrab; i++) {

            /* An error occurred */
            if( ret == -1 ) {
            pcap_perror(handle, "Error processing packet:");
            pcap_close(handle);
            return -1;
            }

            /* Unexpected return values; other values shouldn't happen when reading trace files */
            else if( ret != 1 ) {
            fprintf(stderr, "Unexpected return value (%i) from pcap_next_ex()\n", ret);
            pcap_close(handle);
            return -1;
            }

            // Process the packet and print results 
            else {
                printMAC(packet);
                packetType = checkType(packet);

                // print the appropriate packet info 
                // based on the packet type
                // eg. IPv4, IPv6, ARP, VLAN, or Other
                switch (packetType)
                {
                    case 0x0800: // IPv4
                    errorCode = printIPv4info(packet);
                    if (errorCode == -1)
                    {
                        printf("error printing IPv4 information\n"); 
                        return -1;
                    }
                    break;
                    case 0x86dd: // IPv6
                    errorCode = printIPv6info(packet);
                    if (errorCode == -1)
                    {
                        printf("error printing IPv6 information\n");
                        return -1;
                    }
                    break;
                    case 0x0806: // ARP
                    errorCode = printARPinfo(packet);
                    if (errorCode == -1)
                    {
                        printf("error printing ARP information\n");
                        return -1;
                    }
                    break;
                    case 0x8100: // VLAN
                    errorCode = printVLANinfo(packet);
                    if (errorCode == -1)
                    {
                        printf("error printing VLAN information\n");
                        return -1;
                    }
                    break;
                    default: // Other
                    printf("[Other]");
                    break;
                }   //end switch
            }   //else - no errors
            printf("\n");
            ret = pcap_next_ex(handle, &header, &packet);
        }   //end for
    /* And close the session */
    pcap_close(handle);
    }   //end else
    return 0;
}


