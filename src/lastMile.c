#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <netdb.h>

#include "dns_rr_manipulation.h"
#include "utility_functions.h"

extern  u_short _getshort();
extern  u_long  _getlong();

//typedef union msg dnsmsg; 
#define DEFAULT_COUNT  4
#define DEFAULT_DIR    'b'

char NS1[NSLIMIT][MAXDNAME];
char NS2[NSLIMIT][MAXDNAME];
char RNS[NSLIMIT][MAXDNAME]; // recursive name server
char CNS[MAXDNAME];
int NSCOUNT1 = 0, NSCOUNT2 = 0, RNSCOUNT = 0;
struct in_addr INADDR;
int NS1_OR_NS2 = 1, INFINITY = 100000000;


inline void Usage(int argc, char *argv[]) {
  printf("Usage: %s [-c count] [-d dir] IP_Address1 IP_Address2\n\n", argv[0]);
  printf(" Where count = number of query packets sent, the larger the better, default count = 4 (strongly advised), min 2 \n dir = f for IP_Address1 to IP_Address2 only and  dir = b for either direction, Default dir = b\n");
  printf("Example:\n%s -c 5 -d f 128.95.4.1 128.95.2.67\n", argv[0]);
  exit(-1);
}


int main(int argc, char *argv[]) {
  union dnsmsg response;
  int responselen;
  char * CNS = "ns3.ece.northwestern.edu";
  //int n = resolve(NULL,CNS, T_SOA, &response, &responselen);
  //printf("the resolve time is %d\n", n);
  struct timeval timer_start, timer_end;
  int querytime;

  gettimeofday(&timer_start, NULL);
  char  ping[100];
  strcat(ping, "ping -s 1000 -c 3 ");
  strcat(ping, argv[1]);
  strcat(ping, " | grep icmp");
  puts(ping);
  FILE *cmd = popen ( ping, "r" );//ping google
  char *s = malloc ( sizeof ( char ) * 200 );
  char latency[10];
  while ( 1 ){
            fgets ( s, sizeof ( char )*200, cmd );
            printf ( "%s", s);//show outcome
            if ( strstr ( s, "icmp_req=3" ) != 0 ){
                    //memcpy(latency, s[index], 10);
		    //printf("\n%s\n", latency);
		    char* index = strstr(s, "time=");
            	    index += 5;
	    	    char lat[40];
	            strncpy(lat, index, 4);
	            lat[4] = '\0';
                    float bandwidth = 1008.0 / atof(lat);
	            printf("current bandwidth is %.1fKBps\n", bandwidth);
	            break;
 	    }
  }
  pclose ( cmd );
  gettimeofday(&timer_end, NULL);
  querytime = (timer_end.tv_usec - timer_start.tv_usec)/1000; 
  querytime+= (timer_end.tv_sec - timer_start.tv_sec)*1000; 
 
  printf("Query Time %d msec \n", querytime);   
} 
