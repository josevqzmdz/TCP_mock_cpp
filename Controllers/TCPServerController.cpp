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
    this->tcp_client_service_manager = new TCPClientServiceManager(this);
    this->messaged_type = TCP_DEMARCAR_FIXED_SIZE;
    pthread_mutex_init (&this->message_queue_mutex, NULL);
    // TODO: is cv here "control view" ????
    pthread_condition_init (&this-> message_queue_cv, NULL);
    pthread_readwritelock_init (&this->connect_db_readwritelock, NULL)
    this->state_flags = 0;
    this->SetBit(TCP_SERVER_INITIALIZED);

}// end of constructor

// constructor's destructor
// basically checks if the following objects are NOT null
TCPServerController::~TCPServerController(){
    assert(!this->tcp_new_connection_account);
    assert(!this->tcp_client_database_manager);
    assert(!this->tco_client_service_manager);
    assert(!this->establishedClient.empty());
    printf("The TCP server controller %s stopped. \n", this->name.c_str());
}// end of destructor

/*

    this is a void pointer for the constructor, static defines that this pointer is limited
    to this function's scope (private in modern programming), and this pointer can
    point to any data type but cannot be directly deferenced. Since the exact
    data type here is unknown, it must be versatile.

    this is a static function that serves as a thread cleanup handler / finalization
    routine for the TCP server's message queue processing thread.

    TCPServerController *tcp_ctrlr = (TcpServerController *)arg; -> This casts the void
    pointer argument back to its actual type. This thing passes the thread, a pointer
    to a TCP server controller object.

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL); -> enables the cancel routine
    for this thread. NULL means the code does not care abot the previous cancel state

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL); -> means cancel requests 
    will only be acted upon at specific cancel points, like a system call

    tcp_ctrlr->Message_Queue_Processing_Thread_Fin(); -> calls the message_queue_proce
    ssing_thread_fin method on the TCP server controller object. 
    
    And finally returns null
*/
static void *
tcp_server_message_query_thread_fin (void *arg){
    TCPServerController *tcp_ctrlr = (TCPServerController *)arg;
    pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype (PTHREAD_CANCEL_DEFERRED, NULL);
    tcp_ctrlr->Message_Queue_Processing_Thread_Fin();
    return NULL;
}// end of void pointer

/*

    this is the "start" method for the controller. 

*/
void 
TCPServerController::Start() {
    assert(this->tcp_new_connection_account);
    assert(this->tcp_client_database_manager);
    assert(this->tco_client_service_manager);

    if (!this->IsBitSet (TCP_SERVER_NOT_ACCEPTING_NEW_CONNECTIONS)){
        this->tcp_new_connection_account->StartTCPNewConnectionAcceptorThread();
    }

    if (!this->IsBitSet (TCP_SERVER_NOT_LISTENING_CLIENTS)){
        this->tcp_client_service_manager->StartTCPClientServiceManagerThread();
    }

    // initialize and start TCP server message query thread
    pthread_create (&this->msgO__op_thread, NULL, tcp_server_message_query_thread_fin, (void *)this);

    this->SetBit(TCP_SERVER_RUNNING);

    printf("TCP server is up and running!!! [%s, %d].\n Ok. \n",
        network_convert_ip_n_to_p(this->ip_address, 0), this->port_number
    );
}// end of start method
