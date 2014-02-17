#include <iostream>
#include <string>
#include "../include/enet/enet.h"



int main(int argc, char const *argv[])
{
  if (enet_initialize () != 0)
    {
        return EXIT_FAILURE;
    }
  atexit(enet_deinitialize);

  ENetAddress address;
  ENetHost* server;

  address.host = ENET_HOST_ANY;
  address.port = 1234;

  server = enet_host_create(& address, 32, 2, 0 , 0);
  

  ENetEvent event;

  while (enet_host_service(server, &event, 500000) > 0)
  {
    switch (event.type)
    {
      case ENET_EVENT_TYPE_CONNECT:
          std::cout << "Connection from " << event.peer->address.host << std::endl;
    break;

      case ENET_EVENT_TYPE_RECEIVE:
        std::cout << "Message: " << event.packet->data << std::endl;
        enet_packet_destroy(event.packet);
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        std::cout << "Disconnected: " << event.peer->data;
        event.peer->data = NULL;
        break;

    }
  }
   
  std::cout << "5 sek har gått på serv" << std::endl;

  enet_host_destroy(server);
  
  return 0;
}
