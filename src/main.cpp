#include "Engine.hpp"

int main(int argc, char *argv[])
{
    Engine engine(480, 640);

    engine.SetupObject();

    engine.MainLoop();

    engine.Shutdown();

    return 0;
}