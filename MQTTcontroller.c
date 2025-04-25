#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to trim whitespace from a string
void trim(char *str) {
    char *start = str;
    char *end;

    // Trim leading spaces
    while(isspace((unsigned char)*start)) start++;

    if(*start == 0) {  // All spaces?
        *str = 0;
        return;
    }

    // Trim trailing spaces
    end = start + strlen(start) - 1;
    while(end > start && isspace((unsigned char)*end)) end--;

    // Write new null terminator
    *(end + 1) = 0;

    // If there's still content, move it to the beginning of the string
    if(start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

int main() {
    char broker_ip[100] = "BROKER_IP";
    char topic[100] = "home/light";
    char command[512];
    char input[10];

    char mosquittoPath[] = "C:\\Progra~2\\mosquitto\\mosquitto_pub.exe";

    printf("MQTT Light Control for Windows\n");
    printf("-----------------------------\n\n");

    // Get broker IP if not already set
    if (strlen(broker_ip) == 0) {
        printf("Enter MQTT broker IP address: ");
        fgets(broker_ip, sizeof(broker_ip), stdin);
        trim(broker_ip);

        if (strlen(broker_ip) == 0) {
            printf("Error: Broker IP cannot be empty.\n");
            return 1;
        }
    }

    printf("\nConnected to broker: %s\n", broker_ip);
    printf("Topic: %s\n\n", topic);
    printf("Commands:\n");
    printf("  '1' or 'on'  - Turn light on\n");
    printf("  '0' or 'off' - Turn light off\n");
    printf("  'q' or 'exit' - Quit the program\n\n");

    while (1) {
        printf("Enter command: ");
        fgets(input, sizeof(input), stdin);
        trim(input);

        // Check for exit command
        if (strcmp(input, "q") == 0 || strcmp(input, "exit") == 0) {
            printf("Exiting program.\n");
            break;
        }

        // Process light commands
        if (strcmp(input, "1") == 0 || strcmp(input, "on") == 0) {
            printf("Executing command: %s\n", command);
            sprintf(command, "%s -h %s -t %s -m \"1\"", mosquittoPath, broker_ip, topic);
            printf("Turning light ON\n");
            system(command);
        }
        else if (strcmp(input, "0") == 0 || strcmp(input, "off") == 0) {
            printf("Executing command: %s\n", command);
            sprintf(command, "%s -h %s -t %s -m \"0\"", mosquittoPath, broker_ip, topic);
            printf("Turning light OFF\n");
            system(command);
        }
        else {
            printf("Unknown command. Please use '1'/'on', '0'/'off', or 'q'/'exit'.\n");
        }

        printf("\n");
    }

    return 0;
}