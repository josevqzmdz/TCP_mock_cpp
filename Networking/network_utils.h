// header interface for network_utils.cpp

#ifndef __NETWORK_UTILS__
#define __NETWORK_UTILS__

// general network utils pointers and interface 

char *
network_convert_ip_n_to_p(uint32_t ip_address. char *output_buffer);

uint32_t network_convert_ip_n_to_p(const char *ip_address);

#endif