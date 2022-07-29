/*
Navn: Alija Cerimagic
Email: acer21@student.aau.dk
Gruppe: B224
Studieretning: Software
*/

/*Included libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Defined a max lenght symbolic constant*/
#define MAX_LEN 256
#define MAX_TEAMS 14
#define MAX_NAME 40
#define ERROR -1

/*Structs that contain varibles used in the program*/
typedef struct Time{
    char weekday[4];
    int date_month;
    int date_day;
    double clock;
} Time;

typedef struct Match{
    char team_1[4];
    char team_2[4];
    int score_1;
    int score_2;
    int audience_amount;
    Time match_time;
} Match;

typedef struct Team{
    char team_name[MAX_NAME];
    int points;
    int goals_scored;
    int goals_recieved;
} Team;

/*Prototypes*/
int get_matches_amount(FILE *);
void file_to_variables(FILE *, Match *, int);
void calculate_match_score(Team *, Match *, int);
void team_create(Team *, Match *, int);
void assign_team_name(char *, Team *);
int team_index(char *, Team *);
int l(char *);
int is_zero(char *);
int team_index_2(int);
void sort_teams(Team *team);
int compare_team(const void *team_one, const void *team_two);
void scoreboard(Team *);
void clearScreen();

int main(void){
    int matches_amount;

    /*Struct definitions*/
    Match *match;
    Team team[MAX_TEAMS];
    
    /*Pointer to a txt file*/
    FILE *file_txt;

    /*Name of file*/
    char *file_name = "kampe-2020-2021.txt";

    /*Open and read file*/
    file_txt = fopen(file_name, "r");

    /*If file doesn't open, it gives an error message and exits*/
    if(file_txt == NULL){
        printf("Error - file not found (#2)");
        exit(EXIT_FAILURE);
    }

    /*Function to get the amount of matches by counting lines in .txt*/
    matches_amount = get_matches_amount(file_txt);

    /*Dynamic allocation of array size*/
    match = (Match*) calloc(matches_amount, sizeof(Match));

    /*Check calloc for error*/
    if(match == NULL){
        printf("ERROR - memory not allocated\n");
        exit(EXIT_FAILURE);
    }
    
    file_to_variables(file_txt, match, matches_amount);
    fclose(file_txt);

    team_create(team, match, matches_amount);
    calculate_match_score(team, match, matches_amount);
    sort_teams(team);

    scoreboard(team);

    return 0;
}

int get_matches_amount(FILE *file_txt){
    int matches_amount = 0;
    char counter;
    
    /*While fgets doesn't encounter end of the file, it keeps on looping and counting up*/
    while((counter = fgetc(file_txt)) != EOF){
        if(counter == '\n'){
            matches_amount++;
        }
    }
    matches_amount++;
    /*Sets the FILE position to the beginning of the FILE*/
    rewind(file_txt);

    return matches_amount;
}

void file_to_variables(FILE *file_txt, Match *match, int matches_amount){
    int i = 0;
   
    /*fscanf reads formatted input from a stream in a for loop
      Assigning data in FILE to variables in contained in structs*/
    for(i = 0 ; i < matches_amount ; i++){
        fscanf(file_txt, " %[a-zA-Z] %d/%d %lf %[a-zA-Z] - %[a-zA-Z] %d - %d %d", 
            match[i].match_time.weekday, &match[i].match_time.date_day, 
            &match[i].match_time.date_month, &match[i].match_time.clock, match[i].team_1, 
            match[i].team_2, &match[i].score_1, 
            &match[i].score_2, &match[i].audience_amount);
    }
}

void calculate_match_score(Team *team, Match *match, int matches_amount){
    int i, cp_t1, cp_t2;

    /*Assigning points according to match score*/
    for(i = 0 ; i < matches_amount ; i++){
        cp_t1 = team_index(match[i].team_1, team);
        cp_t2 = team_index(match[i].team_2, team);

        if(match[i].score_1 > match[i].score_2){
            team[cp_t1].points += 3;
        } else if(match[i].score_1 < match[i].score_2){
            team[cp_t2].points += 3;
        } else{
            team[cp_t1].points++;
            team[cp_t2].points++;
        }
        team[cp_t1].goals_scored = team[cp_t1].goals_scored + match[i].score_1;
        team[cp_t2].goals_scored = team[cp_t2].goals_scored + match[i].score_2;
        team[cp_t1].goals_recieved = team[cp_t1].goals_recieved + match[i].score_2;
        team[cp_t2].goals_recieved = team[cp_t2].goals_recieved + match[i].score_1;
    }
}

void team_create(Team *team, Match *match, int matches_amount){
    int i;
    
    /*Assigning 0 to all values in the team struct and "0" copied as string to team names*/
    for(i = 0 ; i < MAX_TEAMS ; i++){
        strcpy(team[i].team_name, "0");
        team[i].points = 0;
        team[i].goals_scored = 0;
        team[i].goals_recieved = 0;
    }
    
    for(i = 0 ; i < matches_amount ; i++){
        assign_team_name(match[i].team_1, team);
    }
}

void assign_team_name(char *team_name_ins, Team *team){
    int cp;

    cp = team_index(team_name_ins, team);

    if(cp == ERROR){
        cp = l(team_name_ins);

        while(!(is_zero(team[cp].team_name))){
            cp = team_index_2(cp);
        }

        strcpy(team[cp].team_name, team_name_ins);
    }
}

int team_index(char *team_name, Team *team){
    int cp = l(team_name);

    /*Loop while team name isn't zero and isn't compared to be the same as the desired team name -> assign cp incremented value*/
    while(!is_zero(team[cp].team_name) && strcmp(team_name, team[cp].team_name) != 0){
        cp = team_index_2(cp);
    }
    /*If team name wasn't found and cp was therefore assigned the value 0*/
    if(is_zero(team[cp].team_name)){
        return ERROR;
    } else{
        return cp;
    }
}

int l(char *team_name_ins){
    int n_lenght;

    n_lenght = strlen(team_name_ins);

    /*Uses the numerical values of the corresponding letters (addition and modulo) as a return value*/
    return (team_name_ins[0] + team_name_ins[n_lenght / 2] + team_name_ins[n_lenght - 1]) % MAX_TEAMS;
}

/*Function to check, if the variable checked is zero/has not been assigned to something other than 0*/
int is_zero(char *team_name){
    if(strcmp(team_name, "0") == 0){
        return 1;
    } else{
        return 0;
    }
    return 0;
}

/*Check if the cp (checkpoint) variable is lower than the max teams amount and then increment if is, else is returned as 0*/
int team_index_2(int cp){
    if(cp < (MAX_TEAMS - 1)){
        cp++;
    } else{
        cp = 0;
    }
    
    return cp;
}

/*Qsort to sort the teams by comparing (compare_team function) the amount of points each team has*/
void sort_teams(Team *team){
    qsort(team, MAX_TEAMS, sizeof(team[0]), compare_team);
}

int compare_team(const void *team_one, const void *team_two){
    Team *team_1 = ((Team *)team_one);
    Team *team_2 = ((Team *)team_two);

    if(team_1->points > team_2->points){
        return -1;
    } else if(team_1->points < team_2->points){
        return 1;
    } else{
        if(team_1->goals_scored - team_1->goals_recieved > team_2->goals_scored - team_2->goals_recieved){
            return -1;
        } else if(team_1->goals_scored - team_1->goals_recieved < team_2->goals_scored - team_2->goals_recieved){
            return 1;
        } 
    }
    return 0;
}

/*Function to print the scoreboard*/
void scoreboard(Team *team){
    int i;

    clearScreen();

    printf("_______________________________\n");
    printf("|                             |\n");
    printf("|         SCOREBOARD          |\n");
    printf("|_____________________________|\n");
    printf("| Teams    Points     Goals   |\n");
    printf("|                             |\n");
    for(i = 0 ; i < MAX_TEAMS ; i++){
        if(is_zero(team[i].team_name) != 1){
            printf("| %4s   -%5d    %4d -%3d  |\n", 
                    team[i].team_name, team[i].points, 
                    team[i].goals_scored, team[i].goals_recieved);
        }
    }
    printf("|_____________________________|\n");
}

/*Function for clearing the terminal / screen*/
void clearScreen(void) {
    /*system("") prints what is inside the string into the system terminal of the user.
      Since systems don't have similar commands for clearing their respective terminal
      The two most common commands for clearing the terminal are used as an or statement
      So if the first one fails, the second one might execute | @cls = win & clear = linux*/
    system("@cls||clear");
}