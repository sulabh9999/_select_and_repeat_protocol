#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/time.h>
#define timediff 5.00 // timer setting
#define windowsize 5 // window size setting
 struct buffstruct 
{
	int packetno;
	char buff[100];
};

void *pthreadfun1();
void *pthreadfun2();
void *pthreadfun3();
void *pthreadfun4();
void  repeatfun();
struct sockaddr_in client;
struct buffstruct mystr;
int flag1=1,flag2=1,flag3=0,flag4=1,flag5=1,flag6=1,count=1;
void *rettime,*ack1,*retbreak;
int s,n,i,filevar,ack=1,window=1,fsize,recvar;
float times=0,timer;
FILE *fp,*fptmp;


int main()
{  
   
    pthread_t thread1,thread2,thread3,thread4;
    mystr.packetno=0;
    s=socket(AF_INET,SOCK_DGRAM,0);
    client.sin_family=AF_INET;
    client.sin_port=2000;
    client.sin_addr.s_addr=inet_addr("127.0.0.1");
   // client.sin_addr.s_addr=inet_addr("10.42.0.1");
    printf("\nClient ready....\n\n");
    n=sizeof(client);
  
   
 //  printf("Enter the file name to sent:");
   // gets(mystr.buff);
  // fp=fopen(mystr.buff,"r");
    fp=fopen("srclient.c","r");
    fptmp=fp;
    if(fp==NULL)
    {
      printf("file does not esist....bye\n");
      exit(0);
     }
    struct stat status;
    fstat(fileno(fp),&status);
    fsize= (int)status.st_size;
    printf("Size of file : %d\n",fsize);
    
    bzero(&mystr,sizeof(mystr));
   // printf("Enter the file name server will get:");
  //  gets(mystr.buff);
   
   // sendto(s,&mystr,sizeof(mystr),0,(struct sockaddr *) &client,n);
    
   
      
	                  pthread_create (&thread1, NULL,pthreadfun1,NULL);
	  		  pthread_create (&thread2, NULL,pthreadfun2,NULL);
	  		  pthread_create (&thread3, NULL,pthreadfun3,NULL);
	  		  pthread_create (&thread4, NULL,pthreadfun4,NULL);
			 
                          pthread_join(thread1,&retbreak);
			  pthread_join(thread2,&rettime);
			  pthread_join(thread3,NULL);
			  pthread_join(thread4,&ack1);
                           
   
   	            if(ack==-1||(int)ack1==-1)
                       { 
                          pthread_cancel(thread1);
                          pthread_cancel(thread2);
                          pthread_cancel(thread3);
                          pthread_cancel(thread4);
                         // fclose(fp);
                       }
       
       printf("file successfilly sent to server......bi sulabh\n");
       return 0;
}


void *pthreadfun1()
{
	while(1)
	 {
           system("sleep 0.00001");
           if(flag1||flag2)
           {
                         if(flag3) 
			  fseek(fp,(window-1)*100,SEEK_SET);
	 
	                           for(i=window;i<(window+windowsize);i++)
	                               { 
                                     
		                         mystr.packetno=i;
                      	                 filevar=fread(&mystr.buff,1,100,fp); 
                                          
                                         printf("\npacket number is......%d and window is ...%d to %d\n",i,window,window+4);
                                           
              			       if(filevar!=sizeof(mystr.buff))
                  	                {
                                	        	 mystr.packetno=-1;
                                	                 sendto(s,&mystr,sizeof(mystr),0,(struct sockaddr *) &client,n);
                                                        // printf("%s",mystr.buff);
                                                         bzero(&mystr.buff,sizeof(mystr.buff));
                                                         fclose(fp);
                                                         break;                                                   
                        	        }	      						
                                    
                                         sendto(s,&mystr,sizeof(mystr),0,(struct sockaddr *) &client,n); 
                                         bzero(&mystr.buff,100);
                     
                                   }
         		 flag1=0;
         		 flag2=0;
                         flag3=0;
                        
           if(mystr.packetno==-1)
           break;
          }
          
      }
  pthread_exit(0);
}       

void *pthreadfun2()
 {
	 times=0;
	 while(1)
	 {
 	   system("sleep 0.007");  // window timer setting......
 	   timer=(times++)/100;
 	    printf("time is.....%.2f",timer);

            if(!recvar)
             printf("\r");
            else
             printf("\n");

            if(ack==-1)
             break;
          
        }
    pthread_exit(0);
  }   
void *pthreadfun3 ()
{
	while(1)
	  {
	       
            system("sleep 0.0000001");
             
           if(timer<timediff && ack==(window+(windowsize-1)))   // loop success condition...
   		     {             
                               
				 recvar=1;
				 times=0;
                               if((count-1)!=(window+(windowsize-1)))
                               {
                                 flag4=1; 
                                
                                repeatfun();
                               }
                               else
                                 {
         		         count=(window+(windowsize-1));
                                 flag1=1;
                                 flag5=1;
         		         window=window+5;
                                 
         		         printf("time in float is.......................................:%0.2f\n",timer);
                                 }
  		     }
       
           	if(timer>=timediff && ack<(window+(windowsize-1)) && flag5)   
   		     {
      		          printf(" timediff is..%.2f ack is..%d windowsize is..%d time is..%.2f\n",timediff,ack,windowsize,timer);
      		          times=0;
      		          flag1=1;
                          flag3=1;
                          recvar=1;
                          printf("time in float is.......................................:%0.2f\n",timer);
                          printf("sulabh sulabh***********************failes\n");
   		     }
               
	   recvar=0;
           if(ack==-1)
           break;        
      }		  
    pthread_exit(0);
}  
void *pthreadfun4()
{
  while(1)
        {
              if(recvfrom(s,&ack,sizeof(ack),0,(struct sockaddr *) &client,&n)>-1) 
  		       {
  		         recvar=1;
  		         printf("acknoladgement is............................................: %d\n",ack);
	               }
  		       if(ack==count)
                       count++;
                       

  		         recvar=0;
  		         if(ack==-1)
                            {
				printf("acknoladgement is............................................: %d\n",ack);
                                break;
		            }
               
         }
        
       pthread_exit(0);
 }

void repeatfun()
{
   if(flag4)
    { 
      if(flag6)
      printf("failed packet is.....%d\n",count);
      flag6=0;
      flag4=0;
      printf("failed packet is resent......%d\n",count);
      fseek(fptmp,(count-1)*100,SEEK_SET);
      mystr.packetno=count;
      fread(&mystr.buff,1,100,fptmp);
   up:sendto(s,&mystr,sizeof(mystr),0,(struct sockaddr *) &client,n);
      if(ack==count)  
      {
      flag1=1;
      flag6=1;
      bzero(&mystr.buff,100);
      }
      else if(timer==(timediff/windowsize))
      goto up;
    }
}









  
