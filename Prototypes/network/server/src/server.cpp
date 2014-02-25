#include <iostream>
#include <string>
#include <vector>

#include "enet/enet.h"
#include <sstream>
#include <glm/glm.hpp>

#include "Package.h"

void sendToAll(const ENetHost* server)
{
  // std::cout << "Sending to klient: " << std::endl;
  // std::string message = "hej";
  // ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
  
  // Package<glm::vec3> test(PLAYER_POSITION, 1, hej);
  // ENetPacket* packet = enet_packet_create(&test, sizeof(test), ENET_PACKET_FLAG_RELIABLE);

  // for (int i = 0; i < server->connectedPeers; ++i)
  // {
  //   enet_peer_send(&server->peers[i], 0, packet);
  // }

  // enet_packet_destroy(packet);
}

glm::vec3 updatePosition(ENetPacket* packet)
{
  glm::vec3* position = (glm::vec3*) packet->data;
  std::cout << "x: " << position->x << std::endl;
  std::cout << "y: " << position->y << std::endl;
  std::cout << "z: " << position->z << std::endl;

  return *position;
}

void sendPosition(ENetPeer* peer, int player, glm::vec3 position)
{
  Package<glm::vec3> test(PLAYER_POSITION, player, position);
  ENetPacket* packet = enet_packet_create(&test, sizeof(test), ENET_PACKET_FLAG_RELIABLE);
  enet_peer_send(peer, 0, packet);
  // enet_packet_destroy(packet);
}

int main(int argc, char const *argv[])
{
  std::vector<glm::vec3> playerPositions;
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
    while (enet_host_service(server, &event, 10) > 0)
    {
      switch (event.type)
      {
        case ENET_EVENT_TYPE_CONNECT:
        {

          std::cout << "Connection from " << event.peer->address.host << std::endl;
          std::cout << "peerCount:  " << server->connectedPeers << std::endl;
          playerPositions.push_back(glm::vec3(0,0,0));
         
          Package<int> test(ASSIGN_PLAYER_NUMBER, playerPositions.size()-1, playerPositions.size());
          ENetPacket* packet = enet_packet_create(&test, sizeof(test), ENET_PACKET_FLAG_RELIABLE);
          enet_peer_send(event.peer, 0, packet);

          if (server->connectedPeers >= 3)
          {
            Package<int> start(START_GAME, -1, server->connectedPeers);
             ENetPacket* startPacket = enet_packet_create(&start, sizeof(start), ENET_PACKET_FLAG_RELIABLE);
             for (int i = 0; i < server->connectedPeers; ++i)
             {
               enet_peer_send(&server->peers[i], 0, startPacket);
             }
            
          }

         break;
        }

        case ENET_EVENT_TYPE_RECEIVE:
        // glm::vec3* hej = event.packet->data;
        for (int i = 0; i < server->connectedPeers; i++)
        {
          if (&server->peers[i] == event.peer)
          {
            playerPositions[i] = updatePosition(event.packet);
          }
        }
        
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
          for (int i = 0; i < server->connectedPeers; i++)
          {
            if (&server->peers[i] == event.peer)
            {
              playerPositions.erase(playerPositions.begin() + i);
            }
          }

          event.peer->data = NULL;
          std::cout << "peerCount:  " << server->connectedPeers << std::endl;
          break;

      }

      for (int i = 0; i < server->connectedPeers; i++)
      {
        for (int j = 0; j < playerPositions.size(); ++j)
        {
          sendPosition(&server->peers[i], j, playerPositions[j]);
        }
      }
      

      // enet_host_flush(server);
      // std::cout << "1 sek har gått på serv" << std::endl;


    }

    // sendToAll(server);

  }
   
  // std::cout << "5 sek har gått på serv" << std::endl;

  enet_host_destroy(server);
  
  return 0;
}
