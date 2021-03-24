//
// Created by vfs on 29-Jun-20.
//

#ifndef ZOMBIES_CLIENT_H
#define ZOMBIES_CLIENT_H

struct Client {
//	RakNet::RakPeerInterface *raknet;
//	RakNet::SocketDescriptor sd;
//
//	RakNet::PacketLogger pl;
//
//	RakNet::SystemAddress peers[64];

	bool connected;

	ZOMBIES_RESULT connect(const char *ip);
	void get_peers();
	void read();
	void imgui();
};

ZOMBIES_RESULT init_client();

#endif //ZOMBIES_CLIENT_H
