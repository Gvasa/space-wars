#include <iostream>
#include <string>
#include <vector>
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
  std::string message;
  ENetPacket* packet;

  std::vector<ENetPeer> clientlist;

  address.host = ENET_HOST_ANY;
  address.port = 1234;

  server = enet_host_create(& address, 32, 2, 0 , 0);
  

  ENetEvent event;
  ENetPeer tempPeer;

  while(true)
  {
    while (enet_host_service(server, &event, 1000) > 0)
    {
      switch (event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "Connection from " << event.peer->address.host << std::endl;
            tempPeer = *event.peer;
            clientlist.push_back(tempPeer);
            std::cout <<  "Number of clients: " << clientlist.size() << std::endl;
         
          // for (int i = 0; i < clientlist.size(); ++i)
          // {
          //     std::cout << i << std::endl;
          //     message = i;
          //     packet = enet_packet_create(&message, sizeof(message), ENET_PACKET_FLAG_RELIABLE);
          //     enet_peer_send(&clientlist[i], 0, packet);
          // }
              
          

      break;

        case ENET_EVENT_TYPE_RECEIVE:
          std::cout << "Message: " << event.packet->data << std::endl;
          message = "hej";
         packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
         // enet_peer_send(event.peer, 0, packet);
          // for (int i = 0; i < clientlist.size(); ++i)
          // {
          //     enet_peer_send(&clientlist[i], 0, event.packet);
          // }
          std::cout <<  "Size: " << clientlist[0].address.host << std::endl;
          enet_peer_send(&clientlist[0], 0, packet);
          enet_packet_destroy(event.packet);
          break;

        case ENET_EVENT_TYPE_DISCONNECT:
          std::cout << "Disconnected: " << event.peer->data;
          event.peer->data = NULL;
          break;

      }
      // enet_host_flush(server);
      std::cout << "1 sek har gått på serv" << std::endl;


    }
  }
   
  std::cout << "5 sek har gått på serv" << std::endl;

  enet_host_destroy(server);
  
  return 0;
}
