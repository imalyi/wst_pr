#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_USER_NAME 50

typedef struct{
    int max_lucifer;
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
    SHOW_STATISTICS=5,
    SHOW_USERS=6,
    EXIT=7
}TMenu;

TMenu menu();
bool add_user(char *username, TUsers* users);
void init_user_list(TUsers *users);
void add_user_menu(TUsers *users);
void show_users(TUsers *users);
bool save(TUsers *users);
void delete_user_menu(TUsers *users);
bool delete_user(TUsers* users, int id);

int main() {
    TUsers users = {};
    users.users_count = 0;
    //init_user_list(&users);

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
                delete_user_menu(&users);
                break;
            case SETTINGS:
                printf("settings");
            case SHOW_STATISTICS:
                printf("show stat");
                break;
            case SHOW_USERS:
                show_users(&users);
                break;
            case EXIT:
                save(&users);
                exit(0);
                break;}
    }while(1);
    return 0;
}

TMenu menu(){
    int action;
    printf("\nMenu:\n1.Start game\n2. Add user\n3. Delete user\n4. Settings\n4. Show statistic\nShow users\nInput #:\n");
    scanf("%d", &action);
    return action;
}


void add_user_menu(TUsers *users){
    char *username[MAX_USER_NAME];
    printf("Input username:\n");
    scanf("%s", &username);
    bool result;


    result = add_user(&username, users);
    if(result){
        printf("%s added!", username);
    } else{
        printf("%s cant add!", username);
    }
}

bool add_user(char* username, TUsers* users){
    printf("add user got %s;", username);
    TUser user = {'0',0, 0};
    stpcpy(user.name, username);
    user.wins = 0;
    user.looses = 0;
    users->users[users->users_count] = user;
    users->users_count++;
    return true;
}

void show_users(TUsers *users_list){
    printf("List of users:\n");
    for(int i=0; i < users_list->users_count; i++){
        printf("%d. Name: %s | Wins: %d, looses: %d \n", i+1, users_list->users[i].name,
               users_list->users[i].wins,
               users_list->users[i].looses);
    }
}

void init_user_list(TUsers *users){
    FILE *fptr;

    if ((fptr = fopen("data/users.bin","rb")) == NULL){
        printf("Cant read users from data/users.bin");
    }

    fscanf(fptr,"%d", users);
    printf("Value of n=%d", users);
    fclose(fptr);
}

bool save(TUsers *users){
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
    return true;
}

void delete_user_menu(TUsers *users){
    int id;
    printf("Enter user id:");
    show_users(users);
    scanf("%d", &id);
    delete_user(users, id);
}