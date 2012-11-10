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


void diep(char *s)
{
  perror(s);
  exit(1);
}
int main(void)
{
  struct sockaddr_in si_me, si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
    diep("socket");
  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(s, &si_me, sizeof(si_me))==-1)
      diep("bind");


  size_t fs;
  /* open ach channel */
  int r = ach_open(&chan_hubo_ref, HUBO_CHAN_REF_NAME , NULL);
  assert( ACH_OK == r );


  struct hubo_ref H_ref;
  memset( &H_ref, 0, sizeof(H_ref));
  char newBuff[sizeof(H_ref)];
  memcpy(&newBuff, &H_ref, sizeof(H_ref));

  ach_put( &chan_hubo_ref, &H_ref, sizeof(H_ref));



/*
  for (i=0; i<NPACK; i++) {
    if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1)
      diep("recvfrom()");
    printf("Received packet from %s:%d\nData: %s\n\n", 
            inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port), buf);
  }
*/
  close(s);
  return 0;
}
