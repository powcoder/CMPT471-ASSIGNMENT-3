https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

// #define PROTOPORT  20004      /* default protocol port number           */

extern  int      errno;
char    localhost[] =   "localhost";   /* default host name             */
/*------------------------------------------------------------------------
 * Program:   tcpechoclient
 *
 * Purpose:   allocate a socket, connect to a server, open a file, 
 *            send the contents of the file through the socket to the
 *            echo server, read the echoed data from the socket and 
 *            write it into an output file
 *
 * Syntax:    tcpechoclient [[[[host [port]] [hostl]] [infile]] [outfile]] 
 *       host        - name of a host on which server is executing
 *       port        - protocol port number server is using
 *       infile      - input file containing data to be echoed
 *       outfile     - output file into which echoed data is to be written
 *       hostl       - name of host on which the client is executing
 *                     (not used in this application)
 *       lenbuf      - MSS, size of read and write buffers
 *
 * Note:   All arguments are optional.  If no host name is specified,
 *         the client uses "localhost"; if no protocol port is
 *         specified, the client uses the default given by PROTOPORT.
 *------------------------------------------------------------------------
 */
int main(argc, argv)
int   argc;
char   *argv[];
{
   struct   hostent  *ptrh;    /* pointer to a host table entry         */
   struct   protoent *ptrp;    /* pointer to a protocol table entry     */
   struct   sockaddr_in sad;   /* structure to hold an IP address       */

   struct   sockaddr_in6 sad6;

   size_t   lenbuf;            /* length of input and output buffers    */
   int      maxfdp1;           /* maximum descriptor value,             */
   int      sd;                /* socket descriptor                     */
   int      port;              /* protocol port number                  */
   char    *host;              /* pointer to host name                  */
   int      EOFFlag;           /* flag, set to 1 when input file at EOF */
   char     *sendbuf;          /* buffer for data going to the server   */
   char     *recvbuf;          /* buffer for data from the server       */
   FILE    *infile;            /* file descriptor for input file        */
   FILE    *outfile;           /* file descriptor for output file       */
   fd_set   descset;           /* set of file and socket descriptors    */
                               /* for select                            */
   ssize_t  nread;             /* number of bytes read by a read        */
   size_t   nwrite;            /* number of bytes written by a write    */
   int      charsin;           /* number of characters sent out through */
                               /* the socket                            */
   int      charsout;          /* number of characters received through */
                               /* the socket                            */
   int      buffersin;         /* number of buffers sent out through    */
                               /* the socket                            */
   int      buffersout;        /* number of buffers received through    */
                               /* the socket                            */
   int      val;

   /* Initialize variables */
   charsin = 0;
   charsout = 0;
   buffersin = 0;
   EOFFlag = 0;
   buffersout = 0;
   infile = NULL;
   outfile = NULL;
   sendbuf = NULL;
   recvbuf = NULL;
   memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure      */
   sad.sin_family = AF_INET;           /* set family to Internet        */

   sad6.sin6_family = AF_INET6;

   /* Check for command-line arguments                                  */
   /* If there are not arguments print an information message           */
   if (argc <= 1) {   
      fprintf (stderr, "Command line arguments are required\n");
      fprintf (stderr, "In order the required arguments are:\n");
      fprintf (stderr, "server IP address:\n");
      fprintf (stderr, "	Default value localhost\n");
      fprintf (stderr, "server port\n");
      fprintf (stderr, "	  Default value 33455 for IPv4 or 33446 for IPv6\n");
      fprintf (stderr, "client IP address\n");
      fprintf (stderr, "	  Default value localhost\n");
      fprintf (stderr, "the filename of the requested file\n");
      fprintf (stderr, "	  Default value fileToTransfer\n");
      fprintf (stderr, "the size of the send/receive buffer\n");
      fprintf (stderr, "	  Default value 1440 for IPv4 and 1280 for IPv6\n");
      fprintf (stderr, "To accept any particular default replace\n");
      fprintf (stderr, "the variable with a . in the argument list\n");
      exit(0);
   }

   /* Check host argument and assign host name. */
   /* Default filename is inputfile, to use default use ? as argument   */
   /* Convert host name to equivalent IP address and copy to sad. */
   /* if host argument specified   */
   if ( (argc > 1) && strncmp(argv[1],".", 1)!=0) {  
      host = argv[1];     
   } 
   else {
      host = localhost;
   }
   ptrh = gethostbyname(host);

   int use6 = 0;

   if ( ((char *)ptrh) == NULL )
   {
      ptrh = gethostbyname2(host,AF_INET6);

      use6 = 1;
   }

   if ( ((char *)ptrh) == NULL ) {
      fprintf(stderr,"invalid host: %s\n", host);
      // close(fileno(infile));
      // close(fileno(outfile));
      exit(1);
   }

   /* Check command-line argument for buffer size  and extract          */
   /* Default buffer size is 1448                                       */ 
   /* ---to use default use . as argument or give no argument           */
   /* print error message and exit in case of error in reading          */
   if ( (argc > 5) && strncmp(argv[5],".", 1)!=0 ) {   
       lenbuf =  atoi(argv[5]);
   }else{

      if (use6)
      {
         lenbuf = 1280;

      }else{

         lenbuf = 1440;
      }
   }

   sendbuf = malloc(lenbuf*sizeof(int) );
   if (sendbuf == NULL) { 
      fprintf(stderr,"send buffer not created, size %s\n",argv[5]);
      exit(1);
   }
   recvbuf = malloc(lenbuf*sizeof(int) );
   if (recvbuf == NULL) { 
      fprintf(stderr,"receive buffer not created size %s\n",argv[5]);
      free(sendbuf);
      exit(1);
   }

   /* Check command-line argument for output filename and extract       */
   /* Default filename is outputfile                                    */
   /* ---to use default use . as argument or give no argument           */
   /* open filename for writing                                         */
   /* print error message and exit in case of error in open             */
   // if ( (argc > 4) && strncmp(argv[4],".", 1)!=0 ) {   
   //    outfile = fopen( argv[4], "w");
   // } else {
   //    outfile = fopen( "outputfile", "w");
   // }
   // if (outfile == NULL) { 
   //    fprintf(stderr,"output file not created %s\n",argv[4]);
   //    free(sendbuf);
   //    free(recvbuf);
   //    exit(1);
   // }
   
   char* fn = "fileToTransfer";
   /* Check command-line argument for input filename and extract        */
   /* Default filename is inputfile, to use default use . as argument   */
   /* ---to use default use . as argument or give no argument 3 or 4    */
   /* open file for reading                                             */
   /* print error message and exit if file not found                    */
   if ( (argc > 3) && strncmp(argv[3], ".", 1)!=0 ) {   
      // infile = fopen(argv[3], "r");

      fn = argv[3];
   }


   // if (infile == NULL) { 
   //    fprintf(stderr,"input file not found %s\n",argv[3]);
   //    close(fileno(outfile));
   //    free(sendbuf);
   //    free(recvbuf);
   //    exit(1);
   // }

   /* Check command-line argument for protocol port and extract         */
   /* port number if one is extracted.  Otherwise, use the default      */
   /* to use default given by constant PROTOPORT use . as argument      */
   /* Value will be converted to an integer and checked for validity    */
   /* An invalid port number will cause the application to terminate    */
   /* Map TCP transport protocol name to protocol number.               */
   if ( (argc > 2) && strncmp(argv[2],".", 1)!=0) {  
      port = atoi(argv[2]);
   } else {

      if(use6)
      {
         port = 33446;
      }else{

         port = 33455;
      }
   }

   if (port > 0) {
      sad.sin_port = htons((u_short)port);

      sad6.sin6_port = htons((u_short)port);

   } else {
      fprintf(stderr,"bad port number %s\n",argv[2]);
      close(fileno(infile));
      close(fileno(outfile));
      free(sendbuf);
      free(recvbuf);
      exit(1);
   }
   if ( ((long int)(ptrp = getprotobyname("tcp"))) == 0) {
      fprintf(stderr, "cannot map \"tcp\" to protocol number");
      close(fileno(infile));
      close(fileno(outfile));
      free(sendbuf);
      free(recvbuf);
      exit(1);
   }

   

   if(use6)
   {
      memcpy(&sad6.sin6_addr, ptrh->h_addr, ptrh->h_length);
      sd = socket(PF_INET6, SOCK_STREAM, ptrp->p_proto);
   }else{


       memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
       sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
   }


   /* Create a TCP socket, and connect it the the specified server      */ 
   // sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

   // sd = socket(PF_INET6, SOCK_STREAM, ptrp->p_proto);
   if (sd < 0) {
      fprintf(stderr, "socket creation failed\n");
      close(fileno(infile));
      close(fileno(outfile));
      free(sendbuf);
      free(recvbuf);
      exit(1);
   }
   val = IP_PMTUDISC_DONT;
   if(setsockopt(sd, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val) ) < 0 ) {
	printf("Error setting MTU discover A");
   }
   val = lenbuf+12;
   if(setsockopt(sd, IPPROTO_TCP, TCP_MAXSEG, &val, sizeof(val) ) < 0 ){
	printf("Error setting MAXSEG ofption A");
   }


   if (   (use6 ? connect(sd, (struct sockaddr *)&sad6, sizeof(sad6)):connect(sd, (struct sockaddr *)&sad, sizeof(sad)))  < 0) {
      fprintf(stderr,"connect failed\n");
       printf("%s\n", strerror(errno) );
      close(fileno(infile));
      close(fileno(outfile));
      close(sd);
      free(sendbuf);
      free(recvbuf);
      exit(1);
   }

   printf("%s\n", fn);

   sprintf(sendbuf, "%s", fn);

   write(sd, sendbuf, strlen(fn));

          if ( ( nread = read( sd, recvbuf, lenbuf ) ) < 0) {
            fprintf(stderr, "error reading from socket");
          close(sd);
            close(fileno(infile));
            close(fileno(outfile));
            close(sd);
            free(sendbuf);
            free(recvbuf);
            exit(1);
         }

         recvbuf[nread] = 0;

         printf("%s\n", recvbuf);

         // int fileZie = 0;

         // char str [40];

         // sscanf(recvbuf, "%s %*s %d",str, &fileZie);



         // printf("%s file size: %d\n", str, fileZie);

         if(recvbuf[0] == 'F') {

            int curSize = 0;

            outfile = fopen(fn, "wb");

            if(outfile == NULL)
            {
               fprintf(stderr, "COULD NOT OPEN OUTPUT FILE\n");

            }else{

               while(1)
               {
                  nread = read(sd, recvbuf, lenbuf);

                  if(nread <= 0)
                  {
                     break;
                  }

                  curSize += nread;

                  recvbuf[nread] = 0;

                  printf("%s", recvbuf);

                  fwrite(recvbuf, 1, nread, outfile);
               }


               fclose(outfile);
            }

         }



   free(sendbuf);
   free(recvbuf);
   close(sd);

   /* Terminate the client program gracefully. */
   exit(0);
}
 
