#include <iostream>
#include <string>
#include "enet/enet.h"
#include <thread>


int main(int argc, char const *argv[])
{


  if (enet_initialize () != 0)
    {
        return EXIT_FAILURE;
    }
  atexit(enet_deinitialize);

  ENetAddress address;
  ENetHost* client;
  ENetPeer* peer;
  std::string message;
  ENetEvent event;
  int eventStatus;

  if (enet_initialize() != 0)
  {
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  client = enet_host_create(NULL, 1, 2, 57600 / 8, 14400 / 8);

  enet_address_set_host(&address, "localhost");
  char hej[10];
  enet_address_get_host(&address, hej, 10);
  std::cout << hej << std::endl;
  address.port = 1234;
  

  peer = enet_host_connect(client, &address, 2, 0);



  while(true)
  {
    while (enet_host_service(client, &event, 1000) > 0)
    {
      switch (event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
        std::cout << "We got a new connection " << event.peer->address.host << std::endl;
        break;

      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "Message from servv: " << event.packet->data << std::endl;
        //enet_peer_disconnect(peer, 3);
        enet_packet_destroy(event.packet);

        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        
        std::cout << "Disconnected from serv: " << event.peer->data << std::endl;
        event.peer->data = NULL;
        break;

      }
      /*message = "HEJ SERVERHAH!";

      ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
      enet_peer_send(peer, 0, packet);*/

    
    }
  }
  
  
   
 
  std::cout << "5 sek har gått på client" << std::endl;

  enet_host_destroy(client);
   return 0;
}
