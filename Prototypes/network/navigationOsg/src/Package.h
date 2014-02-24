#ifndef PACKAGE_H
#define PACKAGE_H

#define PLAYER_POSITION 1
#define PLAYER_POSITION_TYPE glm::vec3
#define ASSIGN_PLAYER_NUMBER 2

#include <string>

template <class dataFormat>
class Package
{
  public:
    Package(int header, int player, dataFormat data)
    {
      _header = header;
      _player = player;
      _data = data;
    }

    int _header;
    int _player;
    dataFormat _data;
};

#endif      