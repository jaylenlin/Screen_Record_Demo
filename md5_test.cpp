#include "md5.h"

int main(){
    const char * s = "1234";
    uint8_t buf[5] = {0};
    memcpy(buf, s, 5);

    printf("md5:%s\n", MD5(buf, strlen(s)).c_str());
    return 0;
}
