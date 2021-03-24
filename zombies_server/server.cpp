//
// Created by vfs on 27/06/2020
//

extern ServerState *state;
extern double t;

Server::Server() {
//	raknet = RakNet::RakPeerInterface::GetInstance();
//	//RakNet::Packet *packet;
//
//	sd = RakNet::SocketDescriptor(60000, 0);
//
//	raknet->SetMaximumIncomingConnections(10);
//	raknet->AttachPlugin(&cn2);
//
//	raknet->Startup(10, &sd, 1);
//
//	LOG_INFO("Started the server.\n");
}

void Server::send() {
//	// for now send the position of the zombie at 20 Hz
//
//	RakNet::BitStream bs_out;
//	bs_out.Write((RakNet::MessageID) ID_GAME_MESSAGE_POS);
//	bs_out.Write(state->zombies[0].orientation.x);
//	bs_out.Write(state->zombies[0].orientation.y);
//	bs_out.Write(state->zombies[0].orientation.z);
//
//	// TODO: KEEP A LIST OF CONNECTED CLIENTS
//	raknet->Send(&bs_out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_RAKNET_GUID, true);
}

void Server::read() {
//	RakNet::Packet *packet = NULL;
//
//	for (packet = raknet->Receive(); packet; raknet->DeallocatePacket(packet), packet = raknet->Receive()) {
//		switch (packet->data[0]) {
//			case ID_REMOTE_DISCONNECTION_NOTIFICATION: {
//				printf("Another client has disconnected.\n");
//			}
//				break;
//
//			case ID_REMOTE_CONNECTION_LOST: {
//				printf("Another client has lost connection.\n");
//			}
//				break;
//
//			case ID_REMOTE_NEW_INCOMING_CONNECTION: {
//				printf("Another client has connected.\n");
//			}
//				break;
//
//			case ID_CONNECTION_REQUEST_ACCEPTED: {
//				printf("Our connection request has been accepted.\n");
//				printf("DEBUG\n");
//
//				RakNet::BitStream bs_out;
//				bs_out.Write((RakNet::MessageID) ID_GAME_MESSAGE_TIME);
//				bs_out.Write("Hello, sailor!");
//				raknet->Send(&bs_out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
//
//			}
//				break;
//
//			case ID_NEW_INCOMING_CONNECTION: {
//				printf("A connection is incoming.\n");
//			}
//				break;
//
//			case ID_NO_FREE_INCOMING_CONNECTIONS: {
//				printf("The server is full.\n");
//			}
//				break;
//
//			case ID_DISCONNECTION_NOTIFICATION: {
//				printf("A client has disconnected.\n");
//			}
//				break;
//
//			case ID_CONNECTION_LOST: {
//				printf("A client lost the connection.\n");
//			}
//				break;
//
//			case ID_GAME_MESSAGE_TIME: {
//				RakNet::RakString rs;
//				RakNet::BitStream bs_in(packet->data, packet->length, false);
//				bs_in.IgnoreBytes(sizeof(RakNet::MessageID));
//				bs_in.Read(rs);
//				printf("%s\n", rs.C_String());
//			}
//				break;
//
//			default: {
//				printf("Message with identifier %i has arrived.\n", packet->data[0]);
//			}
//				break;
//		}
//	}
}
