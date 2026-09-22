#include "BoidScene.h"

#include "BoidManager.h"

void BoidScene::init()
{
    new BoidManager(100);
}
