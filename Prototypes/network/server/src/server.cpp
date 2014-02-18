#include <iostream>
#include <string>
#include "../include/enet/enet.h"
#include <sstream>

void sendToAll(const ENetHost* server)
{
  std::cout << "Sending to klient: " << std::endl;
  std::string message = "hej";
  ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
  for (int i = 0; i < server->connectedPeers; ++i)
  {
    enet_peer_send(&server->peers[i], 0, packet);
  }

}

int main(int argc, char const *argv[])
{
  bool shouldSend = false;
  if (enet_initialize () != 0)
    {
        return EXIT_FAILURE;
    }
  atexit(enet_deinitialize);

  ENetAddress address;
  ENetHost* server;
  std::string message;
  ENetPacket* packet;

  address.host = ENET_HOST_ANY;
  address.port = 1234;

  //int peerCount = 0;

  server = enet_host_create(& address, 32, 2, 0 , 0);
  

  ENetEvent event;

  while(true)
  {
    while (enet_host_service(server, &event, 100) > 0)
    {
      switch (event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
            std::cout << "Connection from " << event.peer->address.host << std::endl;
            std::cout << "peerCount:  " << server->connectedPeers << std::endl;
         
     
          

      break;

        case ENET_EVENT_TYPE_RECEIVE:
          std::cout << "Message: " << event.packet->data << std::endl;
         sendToAll(server);
         // enet_peer_send(event.peer, 0, packet);
         // std::cout << "peerCount:  " << server->connectedPeers << std::endl;
         // int peerNr;
         // for (int i = 0; i < server->connectedPeers; ++i)
         //  {
         //      if (&server->peers[i] == event.peer)
         //        peerNr = i;
         //  }
         //  if (true)
         //  {
         //    std::stringstream ss; 
         //  ss << "Sent by nr: " << peerNr;
          
         // packet = enet_packet_create(ss.str().c_str(), ss.str().length() + 1, ENET_PACKET_FLAG_RELIABLE);
         //  // for (int i = 0; i < server->connectedPeers; ++i)
         //  // {
         //  //     enet_peer_send(&server->peers[i], 0, packet);
         //  // }
         //  }

        //  std::cout <<  "Size: " << clientlist[0].address.host << std::endl;
          enet_packet_destroy(event.packet);
          
          break;

        case ENET_EVENT_TYPE_DISCONNECT:
          std::cout << "Disconnected: " << event.peer->data;
          event.peer->data = NULL;
          std::cout << "peerCount:  " << server->connectedPeers << std::endl;
          break;

      }
      // enet_host_flush(server);
      // std::cout << "1 sek har gått på serv" << std::endl;


    }

    
  }
   
  std::cout << "5 sek har gått på serv" << std::endl;

  enet_host_destroy(server);
  
  return 0;
}
