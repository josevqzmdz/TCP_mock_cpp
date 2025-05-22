/*
    this gigantic fucking thing controlls the rest 
    of the subroutines and interacts with the
    client application whenever it requires TCP packets
*/

#include <assert.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "TCPServerController.h"
// in the official repo its TcpNewCOnnectionAcceptor
#include "../Connections/TCPNewConnectionAccept.h"
#include "../DBMS/TCPClientDBManager.h"
#include "../DRS/TCPClientServiceManager.h"
// TODO: figure out where to create & save this
#include "TCPMsgDemarcar.h"
#include "../Networking/network_utils.h"
#include "../Clients/TCPClient.h"

// TODO: figure out what this does
class TCPMsgDemarcar;

// TCPController's constructor
TCPServerController::TCPServerController(std::string ip_address, uint16_t port_number, std::string name){

    this->ip_address = network_convert_ip_p_to_n(ip_address.c_str());
    this->port_number = port_number;
    this->name = name;
    this->tcp_new_connection_account = new TCPNewConnectionAccept(this);
    this->tcp_client_database_manager = new TCPClientDBManager(this);
    this->tco_client_service_manager = new TCPClientServiceManager(this);
    this->messaged_type = TCP_DEMARCAR_FIXED_SIZE;
    pthread_mutex_init (&this->message_queue_mutex, NULL);
    // TODO: is cv here "control view" ????
    pthread_condition_init (&this-> message_queue_cv, NULL);
    pthread_readwritelock_init (&this->connect_db_readwritelock, NULL)
    this->state_flags = 0;
    this->SetBit(TCP_SERVER_INITIALIZED);

}// end of constructor