#include "sgct.h"

void draw();
void preSync();
void postSyncPreDraw();
void init();
void encode();
void decode();

sgct::Engine* _engine;


int main(int argc, char *argv[])
{
  _engine = new sgct::Engine(argc, argv);

  _engine->setDrawFunction(draw);
  _engine->setPreSyncFunction(preSync);
  _engine->setPostSyncPreDrawFunction(postSyncPreDraw);
  _engine->setInitOGLFunction(init);

  sgct::SharedData::instance()->setEncodeFunction(encode);
  sgct::SharedData::instance()->setDecodeFunction(decode);

  if (!_engine->init())
  {
    delete _engine;
    return EXIT_FAILURE;
  }

  _engine->render();

  delete _engine;

  exit(EXIT_SUCCESS);


  return 0;
}


void draw()
{

}

void preSync()
{

}

void postSyncPreDraw()
{

}

void init()
{

}

void encode()
{

}

void decode()
{

}