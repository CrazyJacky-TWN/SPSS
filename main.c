#include <stdio.h>
#include <stdint.h>
#include "i2c_message.h"








int main(void) {
	Protocol_SupportInit();
  	printf("Hello World\n");
	Protocol_check();
  	return 0;
}