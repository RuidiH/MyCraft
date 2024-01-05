# Run with sh mingw64build.sh


COMPILE="g++ -D MINGW -std=c++17 -shared -fPIC -static-libgcc -static-libstdc++ -I./include/ -I./pybind11/include/ `python3.10 -m pybind11 --includes` ./include/glad/glad.c ./src/*.cpp ./export/*.cpp -o GameEditor.pyd `python3.10-config --ldflags` -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lopengl32 -mwindows -L libwinpthread-1.dll"


echo $COMPILE
echo "-------------------------------------------"
eval $COMPILE

# python3.10 GameEngine.py -m ./py/mygameengine.pyd
