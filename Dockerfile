FROM ubuntu:latest

COPY . /app

WORKDIR /app

RUN apt-get update && \
    apt-get install -y gcc g++ cmake git && \
    git clone https://github.com/libsdl-org/SDL.git && \
    cd SDL && \ 
    mkdir build && \
    cd build  && \
    cmake -DSDL_STATIC=ON -DSDL_SHARED=ON .. && \
    cmake --build . && \
    cmake --install . && \
    cd ../../ && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build .
        
CMD /app/build/homework-01/hello/Main && /app/build/homework-01/hello-with-library/SharedMain && \
    /app/build/homework-01/hello-with-library/StaticMain && \
    /app/build/homework-02/my_sdl_shared && /app/build/homework-02/my_sdl_static
