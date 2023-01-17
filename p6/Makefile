# ***********************************************/
# 
# Author: Bryce McWhirter
# Assignment: Program 6, 7, & 8 (Makefile)
# Course: Intro to Operating Systems
# 
#***********************************************/


CFLAGS = -Wall -Wextra -Wconversion -Wno-unused-parameter
LIB = lib
INCLUDE = -Iinclude


default: rpcserver rpctestdriver hoofsrpc


rpcserver: rpcserver.o
	gcc rpcserver.o -o rpcserver $(LIB)/libxmlrpc_server_abyss.a $(LIB)/libxmlrpc_server.a $(LIB)/libxmlrpc_abyss.a $(LIB)/libxmlrpc.a $(LIB)/libxmlrpc_xmlparse.a $(LIB)/libxmlrpc_xmltok.a $(LIB)/libxmlrpc_util.a -lpthread -lcrypto -lssl


rpcserver.o: rpcserver.c rpcserver.h
	gcc -c $(INCLUDE) $(CFLAGS) rpcserver.c 


rpctestdriver: rpctestdriver.o 
	gcc rpctestdriver.o -o rpctestdriver $(LIB)/libxmlrpc_client.a $(LIB)/libxmlrpc.a $(LIB)/libxmlrpc_xmlparse.a $(LIB)/libxmlrpc_xmltok.a $(LIB)/libxmlrpc_util.a -lcurl -lpthread


rpctestdriver.o: rpctestdriver.c
	gcc -c $(INCLUDE) $(CFLAGS) rpctestdriver.c
	



hoofsrpc: hoofsrpc.o cache.o
	gcc  `pkg-config fuse --cflags` cache.o hoofsrpc.o -o hoofsrpc -DCACHING=1 $(LIB)/libxmlrpc_client.a $(LIB)/libxmlrpc.a $(LIB)/libxmlrpc_xmlparse.a $(LIB)/libxmlrpc_xmltok.a $(LIB)/libxmlrpc_util.a -lcurl -lpthread `pkg-config fuse --libs`


hoofsrpc.o: hoofsrpc.c
	gcc -c $(INCLUDE) $(CFLAGS) -D_FILE_OFFSET_BITS=64 hoofsrpc.c


cache.o: cache.h cache.c
	gcc -c $(INCLUDE) $(CFLAGS) cache.c


clean: 
	rm rpcserver rpctestdriver hoofsrpc *.o

