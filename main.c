#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>
#include <stdbool.h>

typedef struct {
	int room_c;		// count of rooms
	bool * pd;		// passable data
	int map_w;
	int map_h;
} Map;

typedef struct {
	int room_y;		// y axis of room
	int room_x;		// x axis of room
	int room_w;		// width of room
	int room_h;		// height of room
} Room;

typedef struct {
	int player_y;	// y axis on player
	int player_x;	// x axis on player
	int health;		// health of player
} Player;

// prototype functions
Map * map_generator();
Room * room_generator();
Player * player_setup();
bool is_passable(int y, int x, const Map * map_check);
void draw_room(Room * new_room);
int handle_input(int input, Player * user);
int player_move(int y, int x, Player * user);

int main() {

	initscr();
	noecho();
	refresh();

	int ch;

	Player * user;

	user = player_setup();
	Map * new_map = map_generator();
	is_passable(new_map->map_h, new_map->map_w, new_map);
	//Room * r = room_generator();
	draw_room(new_map);

	while ((ch = getch()) != 'q') {
		handle_input(ch, user);
	}

	endwin();

	return 0;
}

// generates new rooms
Room * room_generator() {

	srand(time(NULL));
	Room * new_room = malloc(sizeof(Room));

	new_room->room_y = rand() % 25;
	new_room->room_x = rand() % 80;
	new_room->room_w = rand() % 15 + 5;
	new_room->room_h = rand() % 15 + 5;

	return new_room;
}

// draws each tile
void draw_room(Room * new_room) {

	int y, x;

	for(x = new_room->room_x; x < new_room->room_x + new_room->room_w; x++) {
		mvprintw(new_room->room_y, x, "-");
		mvprintw(new_room->room_y + new_room->room_h - 1, x, "-");
	}

	for(y = new_room->room_y + 1; y < new_room->room_y + new_room->room_h - 1; y++) {
		mvprintw(y, new_room->room_x, "|");
		mvprintw(y, new_room->room_x + new_room->room_w - 1, "|");

		for(x = new_room->room_x + 1; x < new_room->room_x + new_room->room_w - 1; x++) {
			mvprintw(y, x, ".");
		}
	}
}

// sets up player
Player * player_setup() {
	Player * new_player = malloc(sizeof(Player));

	new_player->player_y = 5;
	new_player->player_x = 5;

	new_player->health = 20;

	mvprintw(new_player->player_y, new_player->player_x, "&");
	move(new_player->player_y, new_player->player_x);

	return new_player;
}

// handles input
int handle_input(int input, Player * user) {

	switch(input) {
		// up
		case 'k':
			player_move(user->player_y - 1, user->player_x, user);
			break;
		// down
		case 'j':
			player_move(user->player_y + 1, user->player_x, user);
			break;
		// left
		case 'h':
			player_move(user->player_y, user->player_x - 1, user);
			break;
		// right
		case 'l':
			player_move(user->player_y, user->player_x + 1, user);
			break;

		default:
			break;

	}
	return 0;
}

// moves player based on input
int player_move(int y, int x, Player * user) {

	mvprintw(user->player_y, user->player_x, ".");

	user->player_y = y;
	user->player_x = x;

	mvprintw(user->player_y, user->player_x, "&");
	move(user->player_y, user->player_x);

	return 0;
}

// checks if terrain is passable
bool is_passable(int y, int x, const Map * map_check) {

	if(y >= map_check->map_w || y < 0) {
		return 0;
	}

	if(x >= map_check->map_h || x < 0) {
		return 0;
	}

	return map_check->pd[y + x*map_check->map_w];
}

Map * map_generator() {

	Map * new_map = malloc(sizeof(Map));
	new_map->map_w = 100;
	new_map->map_h = 100;
	new_map->pd = malloc(new_map->map_w * new_map->map_h * sizeof(bool));
	Room * new_room = room_generator();

	return new_map;
}
