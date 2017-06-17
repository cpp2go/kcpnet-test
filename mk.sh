

g++ client.cpp -I ../kcpnet/ -L./ -std=c++11 -lkcpnet -lpthread -o client
g++ server.cpp -I ../kcpnet/ -L./ -std=c++11 -lkcpnet -lpthread -o server
