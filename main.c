#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define MAX_USERS 100
#define MAX_USER_NAME 50

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
    SHOW_STATISTICS=5,
    SHOW_USERS=6,
    EXIT,
}TMenu;

TMenu menu();
bool add_user(char *username, TUsers *users);
void init_user_list(TUsers *users);
void add_user_menu(TUsers *users);
void show_users(TUsers *users);
int main() {
    TUsers users;
    users.users_count = 0;
    init_user_list(&users);

    TMenu action;
    do{
        action = menu();

    switch (action) {
        case START_GAME:
            printf("start game");
            break;
        case ADD_USER:
            add_user_menu(&users);
            break;
        case DELETE_USER:
            printf("delete user");
            break;
        case SETTINGS:
            printf("settings");
        case SHOW_STATISTICS:
            printf("show stat");
        case SHOW_USERS:
            show_users(&users);
    }
    } while (action !=EXIT);
    return 0;
}

TMenu menu(){
    int action;
    printf("Menu:\n1.Start game\n2. Add user\n3. Delete user\n4. Settings\n4. Show statistic\nShow users\nInput #:");
    scanf("%d", &action);
    return action;
}


void add_user_menu(TUsers *users){
    char username[MAX_USER_NAME];
    printf("Input username:\n");
    scanf("%s", &username);
    bool result;

    result = add_user(username, &users);
    if(result){
        printf("%s added!", username);
    } else{
        printf("%s cant add!", username);
    }
}

bool add_user(char *username, TUsers* users_list){
    TUser user;
    stpcpy(user.name, username);
    user.wins = 0;
    user.looses = 0;
    users_list->users[4] = user;
//    users_list->users_count++;
    //printf("%s\n", user.name);
    return true;
}

void show_users(TUsers *users_list){
    for(int i=0; i <= users_list->users_count; i++){
        printf("%s", users_list->users[i].name);
        printf("safasfasasfsafasafsa!!!!!");
    }
}

void init_user_list(TUsers *users){
    //TUser user = {'I',0,0};

}