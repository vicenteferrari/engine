//
// Created by vfs on 29-Jun-20.
//

extern Client client;
extern GameState *current_state;

ZOMBIES_RESULT Client::connect(const char *ip) {
//	pl.LogHeader();
//
//	raknet->Connect(ip, 60000, 0, 0);
//
//	LOG_INFO("Connected to server\n")
//
	return ZOMBIES_SUCCESS;
}

void Client::get_peers() {
//	u16 count = 64;
//	raknet->GetConnectionList(peers, &count);
}

void Client::read() {
//	RakNet::Packet *packet = NULL;
//
//	for (packet = raknet->Receive(); packet; raknet->DeallocatePacket(packet), packet = raknet->Receive()) {
//		switch (packet->data[0]) {
//			case ID_REMOTE_DISCONNECTION_NOTIFICATION: {
//				printf("Another client has disconnected.\n");
//			}
//			break;
//
//			case ID_REMOTE_CONNECTION_LOST: {
//				printf("Another client has lost connection.\n");
//			}
//			break;
//
//			case ID_REMOTE_NEW_INCOMING_CONNECTION: {
//				printf("Another client has connected.\n");
//			}
//			break;
//
//			case ID_CONNECTION_REQUEST_ACCEPTED: {
//				connected = true;
//				printf("Our connection request has been accepted.\n");
//				printf("DEBUG\n");
//
//				RakNet::BitStream bs_out;
//				bs_out.Write((RakNet::MessageID) ID_GAME_MESSAGE_TIME);
//				bs_out.Write("Hello, sailor!");
//				raknet->Send(&bs_out, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
//
//			}
//			break;
//
//			case ID_NEW_INCOMING_CONNECTION: {
//				printf("A connection is incoming.\n");
//			}
//			break;
//
//			case ID_NO_FREE_INCOMING_CONNECTIONS: {
//				printf("The server is full.\n");
//			}
//			break;
//
//			case ID_DISCONNECTION_NOTIFICATION: {
//				printf("We have been disconnected.\n");
//			}
//			break;
//
//			case ID_CONNECTION_LOST: {
//				printf("Connection lost\n");
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
//			break;
//
//			case ID_GAME_MESSAGE_POS: {
//				RakNet::BitStream bs_in(packet->data, packet->length, false);
//				bs_in.IgnoreBytes(sizeof(RakNet::MessageID));
//				bs_in.Read(current_state->zombies[0].n_zombie.orientation.x);
//				bs_in.Read(current_state->zombies[0].n_zombie.orientation.y);
//				bs_in.Read(current_state->zombies[0].n_zombie.orientation.z);
//			}
//			break;
//
//			default: {
//				printf("Message with identifier %i has arrived.\n", packet->data[0]);
//			}
//			break;
//		}
//	}
}

void Client::imgui() {
	if (ImGui::Begin("Connect to server")) {
		static int32_t error;

		static char buf1[256] = "127.0.0.1";
		ImGui::InputText("IP", buf1, 64);

		if (ImGui::Button("Connect")) {
			error = connect(buf1);
		}

		if (ImGui::Button("Send message")) {
			//send();
		}

		if (ImGui::Button("Get peers")) {
			get_peers();
		}

//		if (client.state == DISCONNECTED) {
//			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Status: DISCONNECTED");
//		}

//		if (client.state == CONNECTING) {
//			ImGui::Text("Status: CONNECTING");
//		}

//		if (client.state == CONNECTED) {
//			ImGui::Text("Status: CONNECTED");
//		}
//
//		if (client.state == CONNECTED) {
//			ImGui::Text("Client ID: %d", client.clientId);
//		}

//		if (error == 1) {
//			ImGui::Text("Could not connect because the client is already connected.");
//		}
	}
	ImGui::End();
}

ZOMBIES_RESULT init_client() {
//	client.raknet = RakNet::RakPeerInterface::GetInstance();
//
//	client.raknet->AttachPlugin(&client.pl);
//
//	client.raknet->Startup(1, &client.sd, 1);
//
//	client.connected = false;

	return ZOMBIES_SUCCESS;
}
