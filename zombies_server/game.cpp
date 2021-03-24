//
// Created by vfs on 30-Jun-20.
//

extern Server server;
extern ServerState *state;

void update(float delta) {
	server.read();

	state->update(delta);
}
