 /**
 * @file   testebbchar.c
 * @author Derek Molloy
 * @date   7 April 2015
 * @version 0.1
 * @brief  A Linux user space program that communicates with the ebbchar.c LKM. It passes a
 * string to the LKM and reads the response from the LKM. For this example to work the device
 * must be called /dev/ebbchar.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256               ///< The buffer length (crude but fine)
static char receive[BUFFER_LENGTH];     ///< The receive buffer from the LKM

int main(){
    char buffer[3];
   int ret, fd;
   char stringToSend[BUFFER_LENGTH];
   printf("Starting device test code example...\n");
   
   
   while((strcmp(buffer,"1\n") != 4)){
       printf("Choose 1 for encrypt, 2 for decrypt,\n3 for writing on encrypt then text on               decrypt\n, 4 to exit\n");
   fgets(buffer,3+1,stdin);
 
   if(strcmp(buffer,"1\n") == 0){
       fd = open("/dev/caesare", O_WRONLY);             // Open the device with read/write access
    if (fd < 0){
      perror("Failed to open the device...");
      return errno;
    }
    printf("Type in a string to send to the kernel module, exit with \"!exit\"\n");
    while(strcmp("!exit\n",stringToSend) != 0){
    fgets(stringToSend,BUFFER_LENGTH+1,stdin);                // Read in a string (with spaces)
    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
    if (ret < 0){
        perror("Failed to write the message to the device.");
        return errno;
    }
   }
   }
   
   
   
   if(strcmp(buffer,"2\n") == 0){
       fd = open("/dev/caesard", O_WRONLY);             // Open the device with write access
    if (fd < 0){
      perror("Failed to open the device...");
      return errno;
    }
    printf("Type in a string to send to the kernel module, exit with \"!exit\"\n");
    while(strcmp("!exit",stringToSend) != 0){
    fgets(stringToSend,BUFFER_LENGTH+1,stdin);                 // Read in a string (with spaces)
    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
    if (ret < 0){
        perror("Failed to write the message to the device.");
        return errno;
    }
   }
   }
   
   
   
   
   if(strcmp(buffer,"3\n") == 0){
       
    fd = open("/dev/caesare", O_WRONLY);             // Open the device with write access
    if (fd < 0){
      perror("Failed to open the device...");
      return errno;
    }
    printf("Type in a string to send to the encryptor device \n");
    fgets(stringToSend,BUFFER_LENGTH+1,stdin);                 // Read in a string (with spaces)
    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
    if (ret < 0){
        perror("Failed to write the message to the device.");
        return errno;
        }
    close(fd);
    printf("closing now and opening the decryptor \n");
    fd = open("/dev/caesard", O_WRONLY);             // Open the device with read/write access
    if (fd < 0){
      perror("Failed to open the device...");
      return errno;
    }
    printf("Type in a string to send to the decryptor device \n");
    fgets(stringToSend,BUFFER_LENGTH+1,stdin);                 // Read in a string (with spaces)
    ret = write(fd, stringToSend, strlen(stringToSend)); // Send the string to the LKM
    if (ret < 0){
        perror("Failed to write the message to the device.");
        return errno;
        }
    }
    
    close(fd);
    if(strcmp(buffer,"4\n") == 0){
        return 0;
    }
    
   }
    
    return 0;
  
}
