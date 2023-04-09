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
        
CMD /app/build/my_sdl_shared && /app/build/my_sdl_static
