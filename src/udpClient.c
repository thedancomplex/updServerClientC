#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../../hubo-ach/include/hubo.h"
// for ach
#include <errno.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <inttypes.h>
#include "ach.h"


#define BUFLEN 512
#define NPACK 1000
#define PORT 9930


#define SRV_IP "192.168.0.10"
/* diep(), #includes and #defines like in the server */

// ach channels
ach_channel_t chan_hubo_ref;      // hubo-ach


int hubo_debug = 0;

int main(void)
{
  struct sockaddr_in si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
     printf("socket");
   memset((char *) &si_other, 0, sizeof(si_other));
   si_other.sin_family = AF_INET;
   si_other.sin_port = htons(PORT);
   if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
      fprintf(stderr, "inet_aton() failed\n");
      exit(1);
    }



    size_t fs;
    /* open ach channel */
    int r = ach_open(&chan_hubo_ref, HUBO_CHAN_REF_NAME , NULL);
    assert( ACH_OK == r );


    struct hubo_ref H_ref;
    memset( &H_ref, 0, sizeof(H_ref));
    char newBuff[sizeof(H_ref)];
    memcpy(&newBuff, &H_ref, sizeof(H_ref));




    r = ach_get( &chan_hubo_ref, &H_ref, sizeof(H_ref), &fs, NULL, ACH_O_LAST );
    if(ACH_OK != r) {
        if(hubo_debug) {
            printf("Ref ini r = %s\n",ach_result_to_string(r));}
         }
    else{   assert( sizeof(H_ref) == fs ); }


    for (i=0; i<NPACK; i++) {
      printf("Sending packet %d\n", i);
      sprintf(buf, "This is packet %d\n", i);
      //if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
      if (sendto(s, newBuff, sizeof(newBuff), 0, &si_other, slen)==-1)
        printf("sendto()");
    }

    close(s);
    return 0;
}
