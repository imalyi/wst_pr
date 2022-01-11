#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_USER_NAME 50
#define MAX_MOVES_IN_HISTORY 60

typedef enum{
    ASCENDING,
    DESCENDING,
    }TSort;

typedef struct{
    int max_lucifer;
    TSort ranking_sort_direction;
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
    SHOW_USERS=6,
    EXIT=7
}TMenu;

typedef struct{
    TUser *user1;
    TUser *user2;
    TUser *turn;
    int amount_of_lucifers;
    char history[MAX_MOVES_IN_HISTORY];
}TGame;


TMenu menu();
bool add_user(char *username, TUsers* users);
void init_user_list(TUsers *users);
void add_user_menu(TUsers *users);
void show_users(TUsers *users);
bool save_users(TUsers *users);
void delete_user_menu(TUsers *users);
bool delete_user(TUsers* users, int id);

bool change_settings(TSettings* settings, int new_amount_of_lucifers);
void change_settings_menu(TSettings* settings);
bool save_settings(TSettings* settings);

void show_ranking(TUsers* users, TSettings settings);

void start_game_menu(TUsers* users, TSettings* settings);
bool start_game(TGame* game, TUsers* users, TSettings* settings, int user1_id, int user2_id);
int get_user_move(char* name);
TUser* game_logic(TGame* game);


int main() {
    TUsers users = {};
    TSettings settings = {};
    users.users_count = 0;
    init_user_list(&users);

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
            case SHOW_RANKING:
                printf("show stat");
                break;
            case SHOW_USERS:
                show_users(&users);
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
    printf("\n----Menu----\n1.Start game\n2. Add user\n3. Delete user\n4. Settings\n4. Show ranking\n5. Exit\nInput #:\n");
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

bool add_user(char* username, TUsers* users){
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
    }
    printf("\n\n");
    for(int i=0; i < users_list->users_count; i++){
        printf("%d. Name: %s | Wins: %d, looses: %d \n", i+1, users_list->users[i].name,
               users_list->users[i].wins,
               users_list->users[i].looses);
    }
}

void init_user_list(TUsers *users){
    //FILE *fptr;

    //if ((fptr = fopen("data/users.bin","rb")) == NULL){
    //    printf("Cant read users from data/users.bin");
    //}

    //fscanf(fptr,"%d", users);
    //printf("Value of n=%d", users);
    //fclose(fptr);
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
    for(int i=id; i<=users->users_count; i++){
        users->users[i] = users->users[i+1];
    }
    users->users_count--;
    return true;
}

void delete_user_menu(TUsers *users){
    int id;
    printf("Enter user id:");
    show_users(users);
    scanf("%d", &id);
    delete_user(users, id);
}

bool change_settings(TSettings* settings, int new_amount_of_lucifers){
    settings->max_lucifer = new_amount_of_lucifers;
    return true;
}

void change_settings_menu(TSettings* settings){
    int new_amount_of_lucifers;
    int old_amount_of_lucifers = settings->max_lucifer;
    printf("Enter amount of lucifers for game:\n");
    scanf("%d", &new_amount_of_lucifers);

    if(change_settings(settings, new_amount_of_lucifers)){
        printf("Amount of lucifers changed from %d to %d\n", old_amount_of_lucifers, new_amount_of_lucifers);
    }
}

bool save_settings(TSettings *settings){
    FILE *fptr;
    if ((fptr = fopen("data/settings.bin","wb")) == NULL){
        printf("Cant open file 'data/settings.bin'");
        return false;
    }
    fwrite(settings, sizeof(TSettings), 1, fptr);
    fclose(fptr);
}

//DO THIS!!!
void show_ranking(TUsers* users, TSettings settings) {
    if (settings.ranking_sort_direction == ASCENDING) {
        int a;
    }
    for (int i = 0; i < users->users_count; i++) {
        printf("");
    }
}

void start_game_menu(TUsers* users, TSettings* settings){
    int user1_id, user2_id;
    TGame game;
    show_users(users);
    printf("Enter 2 user ids(for example 2 3):\n");
    scanf("%d %d", &user1_id, &user2_id);

    if(!start_game(&game, users, settings, user1_id, user2_id)){
        printf("Error at starting game!");
    }
    TUser* winner = game_logic(&game);
    printf("%s win!", winner->name);
}

TUser* game_logic(TGame* game){
    do{
        int amount_of_lucifers_for_decr = get_user_move(game->turn->name);
        game->amount_of_lucifers -= amount_of_lucifers_for_decr;
        if(strcmp(game->turn->name, game->user1->name)){                  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            game->turn = game->user2;
        }else{
            game->turn = game->user1;
        }
    }while(game->amount_of_lucifers > 0);

    if(strcmp(game->turn->name, game->user1->name)){                  //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        game->user2->looses++;
    }else{
        game->user1->wins++;
    }
    return game->turn; //winner
}

int get_user_move(char* name){
    int amount_of_lucifers;
    printf("%s, your turn! Input amount of lucifers:\n", name);
    scanf("%d", &amount_of_lucifers);
    return amount_of_lucifers;
}

bool start_game(TGame* game, TUsers* users, TSettings* settings, int user1_id, int user2_id){
    if(user1_id > users->users_count || user2_id > users->users_count){
        return false;
    }
    user2_id--;
    user1_id--;

    TUser* user1 = &users->users[user1_id];
    TUser* user2 = &users->users[user2_id];

    game->user1 = user1;
    game->user2 = user2;
    game->turn = user1;
    game->amount_of_lucifers = settings->max_lucifer;

    return true;
}

//TODO: sort ranking
//TODO: save states