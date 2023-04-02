FROM ubuntu:latest

COPY . /app

WORKDIR /app

RUN apt-get update && apt-get install -y gcc g++ cmake

RUN mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . 
    
CMD /app/build/Homework-01/hello/Main && /app/build/Homework-01/hello-with-library/SharedMain && /app/build/Homework-01/hello-with-library/StaticMain
