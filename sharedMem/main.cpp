#include <sys/mman.h>
#include <sys/stat.h>        
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

//compile with g++ -o main main.cpp -lrt

using namespace std;

char* my_shm_connect(char* name, int len){

    int fd = shm_open(name, O_RDWR|O_CREAT, 0644 );
    
    ftruncate(fd, len);  //set the length to 1024, the default is 0, so this is a necessary step
    
    char *ptr = (char *) mmap(NULL, len, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);// map
    
    if (fd < 0){
	    perror ("Cannot create shared memory\n");
	    exit (0);
    }
    return ptr;
}
void send(char* message){
   char *name = new char[strlen("/testing")];
   sprintf(name, "%s", "/testing"); 

   int len = 1024;
   char* ptr = my_shm_connect (name, len);
	
   strcpy(ptr, message); // putting data by just copying
   printf ("Put message: %s\n", message);
   //close(fd);  // close desc, does not remove the segment
   munmap (ptr, len); // this is a bit redundant,
}

void receive(){
	char   *name = new char[strlen("testing")];
    sprintf(name, "%s", "testing");
	int len = 1024;
	char* ptr = my_shm_connect (name, len);
	
	printf ("Got message: %s\n", ptr);
	shm_unlink (name); //this removes the segment from Kernel, this is a necessary clean up
	exit(0);
}

int main()
{
    char *message = new char[strlen("Hola Mundo")];
    sprintf(message, "%s", "Hola Mundo");
    send(message);
    receive();
    return 0;
}