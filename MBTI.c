#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <string.h>

char line[500];

struct data {
    char id[30];  
    char name[100];     
    char mbti[20];      
    double distance; 
};

double calculate_distance(double q[], double p[]) {
    double sum = 0;
    for (int i = 0; i < 8; i++) {
        sum += pow(q[i] - p[i], 2);
    }
    return sqrt(sum);
}

int main() {
    double query[8] = {22.4, 31, 32, 33, 29, 27, 22, 27};
    struct data Top3[3];
    for(int i = 0; i < 3; i++) {
        Top3[i].distance = 1e18;
        strcpy(Top3[i].name, "None");
    }

    FILE *fp = fopen("CSS121 MBTI 2026 - 68.csv", "r");
    if(fp == NULL) return 1;
    

    while(fgets(line, sizeof(line), fp) != NULL) {
        char id[30], name[100], gender[20], mbti[20], nickname[50];
        double scores[8];
        int enneagram;
        int check = sscanf(line, "%[^,],%[^,],%[^,],%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%[^,],%d,%[^\n\r]",
               id, name, gender,
               &scores[0], &scores[1], &scores[2], &scores[3],
               &scores[4], &scores[5], &scores[6], &scores[7],
               mbti, &enneagram, nickname);

        if (check < 12) continue;
        double current_dist = calculate_distance(query, scores);

        if (current_dist < Top3[0].distance) {
            Top3[2] = Top3[1]; Top3[1] = Top3[0];
            Top3[0].distance = current_dist;
            strcpy(Top3[0].name, name); strcpy(Top3[0].mbti, mbti);
        } else if (current_dist < Top3[1].distance) {
            Top3[2] = Top3[1];
            Top3[1].distance = current_dist;
            strcpy(Top3[1].name, name); strcpy(Top3[1].mbti, mbti);
        } else if (current_dist < Top3[2].distance) {
            Top3[2].distance = current_dist;
            strcpy(Top3[2].name, name); strcpy(Top3[2].mbti, mbti);
        }
    }
    fclose(fp);

    printf("The 3 Nearest Neighbors:\n");
    printf("--------------------------------------------------\n");
    for(int i = 0; i < 3; i++) {
        if(strcmp(Top3[i].name, "None") != 0) {
            printf("%d. %-25s | Distance: %.2f | Type: %s\n",
                   i+1, Top3[i].name, Top3[i].distance, Top3[i].mbti);
        }
    }
    char final_type[20];
    if (strcmp(Top3[0].mbti, Top3[1].mbti) == 0 || strcmp(Top3[0].mbti, Top3[2].mbti) == 0) {
        strcpy(final_type, Top3[0].mbti);
    }
    else if (strcmp(Top3[1].mbti, Top3[2].mbti) == 0) {
        strcpy(final_type, Top3[1].mbti);
    }
    else {
        strcpy(final_type, Top3[0].mbti);
    }

    printf("\n--------------------------------------------------\n");
    printf("ผลคาดว่า MBTI ของคุณคือ : [ %s ]\n", final_type);
    printf("--------------------------------------------------\n");
    return 0;
}