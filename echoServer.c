https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
https://powcoder.com
代写代考加微信 powcoder
Assignment Project Exam Help
Add WeChat powcoder
/* #include <stdlib.h> */
#include <arpa/inet.h>

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

#define   PROTOPORT   20004    /* default protocol port number           */
#define   QLEN        24        /* size of request queue                  */

int   visits       =   0;      /* counts client connections              */

int      tcpsd = -1, tcpsd6 = -1;         /* server socket descriptors           */
    int      connfd;               /* client socket descriptor            */

int      val;

 char     *echobuf;             /* buffer for string the server sends  */
    char     *echobuf6; 

void closeSock()
{
   
   close(tcpsd);
   close(tcpsd6);
}

void freeAll()
{
   free(echobuf);
   free(echobuf6);

    echobuf = NULL;
   echobuf6 = NULL;
}

void exitRelease(int i)
{
   closeSock();

   freeAll();

   exit(i);
}

/*------------------------------------------------------------------------
 * Program:   echoserver
 *
 * Purpose:   allocate a TCP and UDP socket and then repeatedly
 *            executes the following:
 *      (1) wait for the next TCP connection or TCP packet or
 *          UDP packet from a client
 *      (2) when accepting a TCP connection a child process is spawned
 *          to deal with the TCP data transfers on that new connection
 *      (2) when a TCP segment arrives it is served by the child process.
 *          The arriving segment is echoed back to the client
 *      (2) when a UDP packet arrives it is echoed back to the client.
 *      (3) when the TCP connection is terminated the child then terminates
 *      (4) go back to step (1)
 *
 * Syntax:    server [[ port ] [buffer size]]
 *
 *       port  - protocol port number to use
 *       buffer size  - MSS of each packet sent
 *
 * Note:      The port argument is optional.  If no port is specified,
 *         the server uses the default given by PROTOPORT.
 *
 *------------------------------------------------------------------------
 */

int setUpSock(int tcpsd, int lenbuf)
{
    // int tcpsd = socket(AF_INET6, SOCK_STREAM, tcpptrp->p_proto);
    // if (tcpsd < 0) {
    //     fprintf(stderr, "tcp socket creation failed\n");
    //     // free(echobuf);
    //     // close(udpsd);
    //     exitRelease(1);
    // }
    // if (bind(tcpsd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
    //     fprintf(stderr,"tcp bind failed\n");
    //     printf("%s\n", strerror(errno) );
    //     // free(echobuf);
    //     // close(udpsd);
    //     // close(tcpsd);
    //     exitRelease(1);
    // }
    if (listen(tcpsd, QLEN) < 0) {
        fprintf(stderr,"listen failed\n");
        // free(echobuf);
        // close(udpsd);
        // close(tcpsd);
        exitRelease(1);
    }
    /* add 12 bytes for extra options in default header */
    val = lenbuf+12;
    if( setsockopt(tcpsd, IPPROTO_TCP, TCP_MAXSEG, &val, sizeof(val) ) < 0 ) {
        printf("ERROR setting MAXSEG TCP-A\n");
    }
    val = IP_PMTUDISC_DONT;
    if( setsockopt(tcpsd, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val) ) < 0 ) {
        printf("ERROR setting MTU DISCOVER option for tcp A\n");
    }

    // val = IP_PMTUDISC_DONT;
    //         if( setsockopt(tcpsd, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val) ) < 0) {
    //             printf("ERROR setting MTU DISCOVER option B");
    //         }

    return tcpsd;
}

void process(int tcpsd, struct sockaddr * x, char* echobuf, int lenbuf)
{
    /* !!!!!!!!!!!!!!!!!!start child process!!!!!!!!!!!!!!!!!!!!!!!*/
            if( ( fork()) == 0) {
                /* accept incoming data from new TCP connection                */
                /* spawn new TCP connection server to service new connection   */
                val = IP_PMTUDISC_DONT;
                if( setsockopt(tcpsd, IPPROTO_IP, IP_MTU_DISCOVER, &val, sizeof(val) ) < 0) {
                    printf("ERROR setting tcp MTU DISCOVER D\n");
                }
                

                socklen_t      len;

                connfd = accept(tcpsd, x, &len);
                
               
                // printf("%s\n", "accept from client");
                
            
                /* close listening sockets                                  */
                // close(tcpsd);
                // close(udpsd);
                closeSock();

                // tval.tv_sec = 5;
                // tval.tv_usec = 0;
                //       if( setsockopt(connfd, SOL_SOCKET, SO_RCVTIMEO, &tval,
                // sizeof(tval) ) < 0 ) {
                //           printf("%s\n", strerror(errno) );
                //     printf("ERROR setting RCVTIMEOUT for tcp\n");
                //        }
                for( ; ; ) {
                    
                    
                    int  nread = read(connfd, echobuf, lenbuf ) ;
                    
                    // echobuf[nread] = 0;
                    // printf("%s\n", echobuf);
                    // printf("read %d\n", nread);

                    if ( nread < 0) {
                        /*  read nread bytes of data from the      TCP socket */
                        if (errno == EINTR){
                            /* Interrupted before read try again later         */
                            fprintf(stderr, "EINTR reading from TCP socket");
                            continue;
                        }
                        else {
                            fprintf(stderr, "error reading from TCP socket");
                            free(echobuf);
                            close(connfd);
                            exit(1);
                        }
                        
                    }
                    else if (nread > 0 ) {
                        /*  echo nread bytes of data extracted from TCP socket*/
                        /*  increment the number of segment received by 1     */
                        /*  increment the number of bytes echoed by nwrite    */

                        // echobuf[nread] = 0;
                        // printf("%s\n", echobuf);

                         FILE * transFile;
                         transFile = fopen ( echobuf , "rb" );

                         if(transFile == NULL)
                         {

                            

                            printf("COULD NOT OPEN REQUESTED FILE: %s\n", echobuf);

                            printf("%s\n", strerror(errno) );

                            sprintf(echobuf, "COULD NOT OPEN REQUESTED FILE");

                            write(connfd, echobuf, strlen(echobuf));

                         }else{


                            fseek(transFile, 0L, SEEK_END);
                            int fileSize = ftell(transFile);
                            printf("%s: FILE SIZE IS %d bytes\n", echobuf, fileSize);
                            sprintf(echobuf, "FILE SIZE IS %d bytes", fileSize);

                            rewind (transFile);



                           // segmentcnt++;
                           // tcpcharcntin += 8*nread;
                            write(connfd, echobuf, strlen(echobuf));
                           // tcpcharcntout += 8*nwrite;

                            read(connfd, echobuf, lenbuf ) ;

                           int i = 0;
                            for (; i + lenbuf < fileSize; i+=lenbuf)
                            {
                               /* code */
                              fread(echobuf, 1, lenbuf, transFile);

                              write(connfd, echobuf, lenbuf);
                            }

                            if(fileSize > i)
                            {
                               fread(echobuf, 1, fileSize - i, transFile);

                               write(connfd, echobuf, fileSize - i);
                            }
                        }

                         break;

                    }
                    else {
                        
                        /*  nread=0, no data extracted, no data to be echoed  */
                        fprintf(stderr, "no data\n");
                        break;
                    }
                }
            
                close(connfd);

                exitRelease(0);

             }
}


int main(argc, argv)
int   argc;
char   *argv[];
{
    // struct   protoent    *udpptrp; /*pointer to tcp protocol table entry  */
    struct   protoent    *tcpptrp; /*pointer to udp protocol table entry  */
    // struct   sockaddr_in sadu;     /* structure to hold server's address  */
    struct   sockaddr_in sad;      /* structure to hold server's address  */
    struct   sockaddr_in6 sad6;      /* structure to hold server's address  */
    struct   sockaddr_in cad;      /* structure to hold client's address  */
    struct   sockaddr_in6 cad6;      /* structure to hold client's address  */  
    
    int      maxfdp1;              /* maximum descriptor plus 1           */
    int      port = 33455;                 /* protocol port number                */
    int      port6 = 33446;
   
    size_t   lenbuf = 1440;               /* length of buffer                    */
    size_t   lenbuf6 = 1280;
    int      segmentcnt;           /* cumulative # of segments received   */
    int      packetcnt;            /* cumulative # of packets received    */
    int      tcpcharcntin;         /* cumulative # of octets received     */
    int      tcpcharcntout;        /* cumulative # of octets sent         */
    int      udpcharcnt;           /* cumulative # of octets received     */
    int      nread;                /* # of octets received in one read    */
    int      nwrite;               /* # of octets sent in one write       */
    int      retval;               /* function return flag for testing    */
    struct timeval tval;           /* max time to wait before next select */
    socklen_t      len;            /* length of the socket address struct */
    pid_t    pid;
    fd_set   descset;
    
    
    /* Initialize variables                                               */
    packetcnt = 0;
    segmentcnt = 0;
    udpcharcnt = 0;
    tcpcharcntin = 0;
    tcpcharcntout = 0;
     memset((char *)&sad6,0,sizeof(sad6)); /* clear sockaddr structure      */
    memset((char *)&sad,0,sizeof(sad)); /* clear sockaddr structure      */
    memset((char *)&cad,0,sizeof(cad)); /* clear sockaddr structure      */
    memset((char *)&cad6,0,sizeof(cad6)); /* clear sockaddr structure      */
    // memset((char *)&sadu,0,sizeof(sadu));/* clear sockaddr structure     */
    sad.sin_family = AF_INET;           /* set family to Internet        */
    sad6.sin6_family = AF_INET6;           /* set family to Internet        */
    sad.sin_addr.s_addr = INADDR_ANY;   /* set the local IP address      */

    sad6.sin6_addr = in6addr_any;
    // sadu.sin_family = AF_INET;          /* set family to Internet        */
    // sadu.sin_addr.s_addr = INADDR_ANY;  /* set the local IP address      */
    cad.sin_family = AF_INET;          /* set family to Internet         */
    cad6.sin6_family = AF_INET6;          /* set family to Internet         */
    
    /* Check for command-line arguments                                  */
    /* If there are not arguments print an information message           */
    if (argc <= 1) {
        fprintf (stderr, "Command line arguments are required\n");
        fprintf (stderr, "In order the required arguments are:\n");
        fprintf (stderr, "server IPv4 port number\n");
        fprintf (stderr, "          Default value 33455\n");
        fprintf (stderr, "sever IPv6 port number\n");
        fprintf (stderr, "          Default value 33446\n");
        fprintf (stderr, "size of send/receive buffer\n");
        fprintf (stderr, "          Default value 1440 for IPv4 and 1280 for IPv6\n");
        fprintf (stderr, "To accept any particular default replace\n");
        fprintf (stderr, "the variable with a . in the argument list\n");
        exit(0);
    }
    
    /* Check command-line argument for buffer size  and extract          */
    /* Default buffer size is 1448                                       */
    /* ---to use default use . as argument or give no argument           */
    /* print error message and exit in case of error in reading          */
    // lenbuf=1400;

    if ( (argc > 3) && strncmp(argv[3],".", 1)!=0 ) {
        lenbuf =  atoi(argv[3]);
        lenbuf6 = lenbuf;
    } 

    echobuf = malloc(lenbuf*sizeof(int) );
    if (echobuf == NULL) {
        fprintf(stderr,"echo buffer not created, size %s\n",argv[3]);
        exit(1);
    }

    echobuf6 = malloc(lenbuf6*sizeof(int) );
    if (echobuf6 == NULL) {
        fprintf(stderr,"echo buffer not created, size %s\n",argv[3]);
        free(echobuf);
        exit(1);
    }


    
    
    /* Check command-line argument for the protocol port and extract      */
    /* port number if one is specified.  Otherwise, use the   default     */
    /* port value given by constant PROTOPORT                             */
    /* check the resulting port number to assure it is valid (>0)         */
    /* convert the valid port number to network byte order and insert it  */
    /* ---  into the socket address structure.                            */
    /* OR print an error message and exit if the port is invalid          */
    
    if (argc > 2 && strncmp(argv[2],".", 1)!=0 ) {
        port6 = atoi(argv[2]);
    } 

     if (port6 > 0) {
         
         printf("port6: %d\n", port6);
         sad6.sin6_port = htons((u_short)port6);
        
    }
    else {
        fprintf(stderr,"bad port number for IPv6 %s\n",argv[2]);
        free(echobuf);
        free(echobuf6);
        exit(1);
    }

    if (argc > 1 && strncmp(argv[1],".", 1)!=0 ) {
        port = atoi(argv[1]);
    } 

    if (port > 0) {
        printf("port: %d\n", port);
        sad.sin_port = htons((u_short)port);
       
    }
    else {
        fprintf(stderr,"bad port number for IPv4 %s\n",argv[1]);
        free(echobuf);
        free(echobuf6);
        exit(1);
    }
    
    
    
    /* Map TCP transport protocol name to protocol number                 */
    /* Create a tcp socket with a socket descriptor tcpsd                 */
    /* Bind a local address to the tcp socket                             */
    /* Put the TCP socket in passive listen state and specify the lengthi */
    /* --- of request queue                                               */
    /* If any of these four processes fail an explanatory error message   */
    /* --- will be printed to stderr and the server will terminate        */
    if ( ((long int)(tcpptrp = getprotobyname("tcp"))) == 0) {
        fprintf(stderr, "cannot map \"tcp\" to protocol number");
        // free(echobuf);
        // close(udpsd);
        // exit(1);

        exitRelease(1);
    }
    tcpsd = socket(AF_INET, SOCK_STREAM, tcpptrp->p_proto);

    tcpsd6 = socket(AF_INET6, SOCK_STREAM, tcpptrp->p_proto);

    if (tcpsd < 0) {
        fprintf(stderr, "tcp socket creation failed\n");
        // free(echobuf);
        // close(udpsd);
        exitRelease(1);
    }
    if (bind(tcpsd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
        fprintf(stderr,"tcp bind failed\n");
        printf("%s\n", strerror(errno) );
        // free(echobuf);
        // close(udpsd);
        // close(tcpsd);
        exitRelease(1);
    }

    if (tcpsd6 < 0) {
        fprintf(stderr, "tcp socket creation failed\n");
        // free(echobuf);
        // close(udpsd);
        exitRelease(1);
    }
    if (bind(tcpsd6, (struct sockaddr *)&sad6, sizeof(sad6)) < 0) {
        fprintf(stderr,"tcp6 bind failed\n");
        printf("%s\n", strerror(errno) );
        // free(echobuf);
        // close(udpsd);
        // close(tcpsd);
        exitRelease(1);
    }

    setUpSock(tcpsd, lenbuf);

    setUpSock(tcpsd6, lenbuf);
    
    
    /* Main server loop - accept and handle requests                     */
    
    /* Define the descriptor set for select, unset all descriptors       */
    /* determine the largest descriptor in use                           */
    FD_ZERO(&descset);
    if (tcpsd6 < tcpsd) maxfdp1 = tcpsd + 1;
        else maxfdp1 = tcpsd6 + 1;
            
        /* Repeatedly check each socket for arriving data                     */
        /* On each pass through the for loop do each of the following:        */
        /* --- set the descriptors for the TCP and UDP sockets in descset     */
        /* --- check each socket for TCP connection requests or UDP data      */
        /* --- process TCP connection requests or incoming UDP data           */
            
    for( ; ; ) {
        FD_SET(tcpsd, &descset);
        FD_SET(tcpsd6, &descset);
        
        if ( (retval = select(maxfdp1, &descset, NULL, NULL, &tval) ) < 0 ) {
            if (errno == EINTR) {
                printf("EINTR; select was interrupted \n");
                continue;
            }
            else if (retval == -1) {
                fprintf(stderr,"select error\n");
            }
            /*  timed out, no descriptors ready */
            else if (retval == 0) {
                if( (pid = waitpid( -1, NULL, WNOHANG) ) > 0) {
                    printf("child process %d terminated \n", pid);
                }
                printf("retval 0\n");
            }
        }
        
        if( (pid = waitpid( -1, NULL, WNOHANG) ) > 0) {
            /*printf("child process %d terminated \n", pid);*/
        }
        
        // int tcpsd_cur = -1;

        if(FD_ISSET(tcpsd, &descset))
        {
           // tcpsd_cur = tcpsd;

           process(tcpsd, (struct sockaddr *)&cad,  echobuf,  lenbuf);

        }

        if(FD_ISSET(tcpsd6, &descset))
        {
           process(tcpsd6, (struct sockaddr *)&cad6,  echobuf6,  lenbuf6);
        }
        
        
    }
}

