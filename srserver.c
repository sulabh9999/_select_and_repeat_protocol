#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#define delly system("sleep 0.8 "); // time difference setting....
#define pfail 3                     // packet to be fail..........
struct datastruct
{
  int packetno;
  char buffer[100];
};
struct datastruct mystr;
int main()
{
    struct sockaddr_in server;
    int s,n,filevar;
    int tmp=0,i=0,k,mytmp=0;
    char buff[100];
    FILE *fp;
    s=socket(AF_INET,SOCK_DGRAM,0);
    server.sin_family=AF_INET;
    server.sin_port=2000;
    server.sin_addr.s_addr=inet_addr("127.0.0.1");
   // server.sin_addr.s_addr=inet_addr("10.42.0.42 ");
    bind(s,(struct sockaddr *)&server,sizeof(server));
    printf("\nclient ready,waiting for packet....\n");
    n=sizeof(server);
    bzero(&mystr,sizeof(mystr));
    
   // recvfrom(s,&mystr,sizeof(mystr),0,(struct sockaddr *) &server,&n);
   // printf("server has got this file:%s\n",mystr.buffer);
    fp=fopen("zz.c","w+");
    
    
    while(1)
    {  
        
          recvfrom(s,&mystr,sizeof(mystr),0,(struct sockaddr *) &server,&n);
           printf("\nclient has got packet....%d\n",mystr.packetno);  
             if(mystr.packetno==pfail && mytmp!=1)
              {
                mytmp++;
                goto down;
              }
              
             if(mystr.packetno==-1)
             {
				  sendto(s,&mystr.packetno,4,0,(struct sockaddr *) &server,n);
				  printf("client sending last packet....%d\n",mystr.packetno);
				 while(1)
				 {
				   if(mystr.buffer[i]=='\0')	 
			           break;
			           fputc(mystr.buffer[i++],fp);
                         	}
                  printf("%s",mystr.buffer);	
		          break;
	    }
           
                           
                  fseek(fp,(mystr.packetno-1)*100,SEEK_SET); 
           	  fwrite(mystr.buffer,sizeof(mystr.buffer),1,fp);
                  // fputs("dfb uygfe hfg yfgg",fp);
             
        	 
        	     sendto(s,&mystr.packetno,4,0,(struct sockaddr *) &server,n); 
			   
	             printf("%s",mystr.buffer);
             
             
           printf("client sent this ack :%d\n",mystr.packetno);
          
         

           bzero(&mystr,sizeof(mystr));
    down:delly;

    }
    printf("bi***************************\n");
    fclose(fp);
    return 0;
}
