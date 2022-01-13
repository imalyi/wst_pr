#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_USER_NAME 14

typedef struct{
    int max_lucifer_in_game;
}TSettings;

typedef struct{
    char name[MAX_USER_NAME];
    int wins;
    int looses;
}TUser;

typedef struct{
    TUser users[MAX_USERS];
    int users_count;
}TUsers;

typedef enum{
    START_GAME=1,
    ADD_USER=2,
    DELETE_USER=3,
    SETTINGS=4,
    SHOW_RANKING=5,
    EXIT=6
}TMenu;

typedef struct{
    TUser *user1;
    TUser *user2;
    TUser *turn;
    TUser *winner;
    int amount_of_lucifers;
}TGame;

TMenu menu();
bool add_user(char *username, TUsers* users);
void init_user_list(TUsers *users);
void add_user_menu(TUsers *users);
void show_users(TUsers *users);
bool save_users(TUsers *users);
void delete_user_menu(TUsers *users);
bool delete_user(TUsers* users, int id);

void change_settings(TSettings* settings, int new_amount_of_lucifers);
void change_settings_menu(TSettings* settings);
void save_settings(TSettings* settings);

bool init_settings(TSettings* settings);

void show_ranking(TUsers* users);

void start_game_menu(TUsers* users, TSettings* settings);
bool start_game(TGame* game, TUsers* users, TSettings* settings, int user1_id, int user2_id);
int get_user_move(char* name);
TUser* game_logic(TGame* game);
void show_game_status(TGame * game);


int main() {
    TUsers users = {};
    TSettings settings = {};
    users.users_count = 0;
    init_user_list(&users);
    init_settings(&settings);
    TMenu action;
    do{
        action = menu();
        switch (action) {
            case START_GAME:
                start_game_menu(&users, &settings);
                break;
            case ADD_USER:
                add_user_menu(&users);
                break;
            case DELETE_USER:
                delete_user_menu(&users);
                break;
            case SETTINGS:
                change_settings_menu(&settings);
                break;
            case SHOW_RANKING:
                show_ranking(&users);
                break;
            case EXIT:
                save_users(&users);
                save_settings(&settings);
                exit(0);
                }
    }while(1);
}

TMenu menu(){
    int action;
    printf("\n----Menu----\n1.Start game\n2. Add user\n3. Delete user\n4. Settings\n5. Show ranking\n6. Exit\nInput #:\n");
    scanf("%d", &action);
    return action;
}

void add_user_menu(TUsers *users){
    char *username[MAX_USER_NAME];

    printf("Input username:\n");
    scanf("%s", &username);
    if(add_user(&username, users)){
        printf("%s added!\n", username);
    }else{
        printf("%s not added!\n", username);
    }
}

bool add_user(char *username, TUsers* users){
    TUser user = {'0',0, 0};

    stpcpy(user.name, username);
    user.wins = 0;
    user.looses = 0;

    for(int i=0; i<users->users_count; i++){
        if(strcmp(username, users->users[i].name) == 0){
            return false;
        }
    }
    users->users[users->users_count] = user;
    users->users_count++;
    return true;
}

void show_users(TUsers *users_list){
    printf("List of users:\n");
    if(users_list->users_count == 0){
        printf("--Empty user list--\n");
        return;
    }
    printf("#. Name\n");
    for(int i=0; i < users_list->users_count; i++){
        printf("%d. %s \n", i+1, users_list->users[i].name);
    }
}

void init_user_list(TUsers *users){
    FILE *fptr = NULL;

    if((fptr = fopen("data/users.bin", "rb")) == NULL) {
        printf("can't open file\n");
        return;
    }
    fread(users, sizeof(TUsers), 1, fptr);
    fclose(fptr);
    printf("Next users was loaded:\n");
    show_users(users);
    if(users->users_count <2){
        printf("There are less then 2 users. Add several!\n");
        do{
            add_user_menu(users);
        } while (users->users_count < 2);
    }
}

bool save_users(TUsers *users){
    FILE *fptr;

    if ((fptr = fopen("data/users.bin","wb")) == NULL){
        printf("Cant open file 'data/users.bin'");
        return false;
    }
    fwrite(users, sizeof(TUsers), 1, fptr);
    fclose(fptr);
}

bool delete_user(TUsers* users, int id){
    id--;
    if(id>users->users_count){
        return false;
    }
    for(int i=id; i<=users->users_count; i++){
        users->users[i] = users->users[i+1];
    }
    users->users_count--;
    return true;
}

void delete_user_menu(TUsers *users){
    int id;

    printf("Enter user id:\n");
    show_users(users);
    scanf("%d", &id);
    if(delete_user(users, id)){
        printf("User %d deleted\n", id);
    }else{
        printf("Cant delete %d\n", id);
    }
}

void change_settings(TSettings* settings, int new_amount_of_lucifers){
    settings->max_lucifer_in_game = new_amount_of_lucifers;
}

void change_settings_menu(TSettings* settings){
    int new_amount_of_lucifers;
    int old_amount_of_lucifers = settings->max_lucifer_in_game;

    printf("Current amount is: %d.Enter new amount of lucifers for game:\n", settings->max_lucifer_in_game);
    scanf("%d", &new_amount_of_lucifers);
    change_settings(settings, new_amount_of_lucifers);
    printf("Amount of lucifers was changed from %d to %d\n", old_amount_of_lucifers, new_amount_of_lucifers);
}

void save_settings(TSettings *settings){
    FILE *fptr;

    if ((fptr = fopen("data/settings.bin","wb")) == NULL){
        printf("Cant open file 'data/settings.bin'");
        return;
    }
    fwrite(settings, sizeof(TSettings), 1, fptr);
    fclose(fptr);
}


void start_game_menu(TUsers* users, TSettings* settings){
    int user1_id, user2_id;
    TGame game;

    show_users(users);
    printf("Enter 2 user ids(for example 2 3):\n");
    scanf("%d %d", &user1_id, &user2_id);

    if(!start_game(&game, users, settings, user1_id, user2_id)){
        printf("Error at starting game!");
        return;
    }
    TUser* winner = game_logic(&game);
    printf("%s win!", winner->name);
}

void show_game_status(TGame * game){
    for(int i=0; i<game->amount_of_lucifers; i++){
        printf("|");
    }
    printf("\n");
}

TUser* game_logic(TGame* game){
    do{
        show_game_status(game);
        int amount_of_lucifers_for_decr = get_user_move(game->turn->name);
        game->amount_of_lucifers -= amount_of_lucifers_for_decr;
        if(strcmp(game->turn->name, game->user1->name) == 0){
            game->turn = game->user2;
        }else{
            game->turn = game->user1;
        }
    }while(game->amount_of_lucifers > 0);

    if(strcmp(game->turn->name, game->user1->name) == 0){
        game->user1->looses++;
        game->user2->wins++;
        game->winner = game->user2;
    }else{
        game->user2->wins++;
        game->user1->looses++;
        game->winner = game->user2;
    }
    return game->winner;
}

int get_user_move(char* name){
    unsigned int amount_of_lucifers;

    printf("%s, your turn! Input amount of lucifers(1-3):\n", name);
    do{
        scanf("%d", &amount_of_lucifers);
        if(amount_of_lucifers >= 4){
            printf("Incorect value. Input number in range 1-3.\n");

        }
        return amount_of_lucifers;
    } while (1);
}

bool start_game(TGame* game, TUsers* users, TSettings* settings, int user1_id, int user2_id){
    user2_id--;
    user1_id--;

    if(user1_id > users->users_count || user2_id > users->users_count){
        return false;
    }

    TUser* user1 = &users->users[user1_id];
    TUser* user2 = &users->users[user2_id];

    game->user1 = user1;
    game->user2 = user2;
    game->turn = user1;
    game->amount_of_lucifers = settings->max_lucifer_in_game;

    return true;
}

bool init_settings(TSettings* settings){
    FILE *fptr = NULL;

    if((fptr = fopen("data/settings.bin", "rb")) == NULL) {
        printf("can't open file\n");
        return false;
    }
    fread(settings, sizeof(TSettings), 1, fptr);
    fclose(fptr);
}

void show_ranking(TUsers* users){
    printf("# NAME    WINS    LOOSES\n");
    for(int i=0; i<users->users_count; i++){
        printf("%d. %s %8d %8d\n", i+1, users->users[i].name, users->users[i].wins, users->users[i].looses);
    }
}
