/*
 * Study material reference:
 * Prof. Mike Shah - https://www.youtube.com/@MikeShah
 * Dr. Jeffrey Paone - CSCI 441
 */

#include "Engine.hpp"

int main(int argc, char *argv[])
{
    Engine engine(1000, 600);

    if (argc == 1 || (argc == 2 && std::string(argv[1]) == "-g"))
    {
        engine.GenerateWorld("./noise_map.csv");
    }
    else if (argc == 2 && std::string(argv[1]) == "-l")
    {
        engine.LoadSavedWorld();
    }
    
    // engine.GenerateWorld("./noise_map.csv");

    engine.MainLoop();

    engine.Shutdown();

    return 0;
}
