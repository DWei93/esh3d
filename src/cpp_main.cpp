#include <iostream>
#include <string.h>
 
extern "C"{
	void read_esh3d_parameters(char *, int *);
    void write_esh3d_parameters(void);
}

 int main() {
	char inpfile[256];
    int length = sizeof(inpfile);
    for(int i=strlen(inpfile);i<length;i++) inpfile[i]=' ';
    snprintf(inpfile,sizeof(inpfile),"esh3d.inp");

	read_esh3d_parameters(inpfile, &length);
    write_esh3d_parameters();
    return 0;
 }
