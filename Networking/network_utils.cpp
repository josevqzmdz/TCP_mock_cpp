// idk what this does honestly kek

#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <memory.h>
#include "network_utils.h"

// this pointer assigns an IP address, from
// literal zeros and ones to decimal values
// it leaves the final bit as zero to 
// assign it later to a host or client or whatever

char *
network_convert_ip_n_to_p(uint32_t ip_address,
    char *output_buffer){

        char *out = NULL;
        static char string_ip[16];
        out = !output_buffer ? string_ip : output_buffer;
        memset(out, 0, 16);
        ip_address = htonl(ip_address);
        inet_ntop(AF_INET, &ip_address, out, 16);
        out[15] = '\0';
        return out;

}// end pointer that returns a char value

// this next method assigns the previous char value 
// to a int_32 integer

uint32_t
network_convert_ip_n_to_p(const char *ip_address){

    uint32_t binary_prefix = 0;
    inet_pton(AF_INET, ip_address, &binary_prefix);
    binary_prefix = htonl(binary_prefix);
    return binary_prefix;

}