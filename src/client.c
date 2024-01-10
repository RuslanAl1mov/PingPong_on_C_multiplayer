#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PORT 8080

void connection_menu(int *connection_variant);
void connect_to_server(char *server_host, int *client_fd, struct sockaddr_in serv_addr, int *status,
                       int *connection_status);
int check_ip_exists(char *ip_address);
void send_message(int client_fd);
void recive_message(int client_fd, int *connection_status);

int main(int argc, char const *argv[]) {
    int connection_variant, status, valread, client_fd;
    int connection_status = 1;
    char server_host[15];
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    connection_menu(&connection_variant);

    switch (connection_variant) {
        case 1:
            break;

        case 2:
            connect_to_server(server_host, &client_fd, serv_addr, &status, &connection_status);
            break;
    }


    while (connection_status) {
        // Send message
        send_message(client_fd);
        recive_message(client_fd, &connection_status);

        scanf("%*[^\n]%*c");
    }

  return 0;
}

void connection_menu(int *connection_variant) {
    /*
        Connection menu.
        Here user can choose being server or client.
        Function changing connection_variant veriable, which 1-being server; 2-being client
    */

    // printf("\033[2J\033[H");
    printf("Welcome!\n 1. Create game\n 2. Connect to your Bro\nSelect option: ");
    if (scanf("%d", connection_variant) != 1 || *connection_variant > 2 || *connection_variant < 1) {
        scanf("%*[^\n]%*c");
        connection_menu(connection_variant);
    }
}

void connect_to_server(char *server_host, int *client_fd, struct sockaddr_in serv_addr, int *status,
                       int *connection_status) {
    /*
        Connect to Server
    */

    printf("Enter your Bro's HOST (example: 127.0.0.1)\nHost: ");
    scanf("%s", server_host);

    if (check_ip_exists(server_host)) {
        printf("\nI can't find PC with this IP: %s\n", server_host);
        *connection_status = 0;
    }

    if ((*client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error\n");
        *connection_status = 0;
    } else {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PORT);

        if (inet_pton(AF_INET, server_host, &serv_addr.sin_addr) <= 0) {
            perror("Invalid address/Address not supported\n");
            *connection_status = 0;
        }

        if ((*status = connect(*client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
            perror("Connection Failed\n");
            *connection_status = 0;
        }
    }
}

void send_message(int client_fd){
    /*
        Connect to Server
    */
   
    char message[200];
    scanf("%s", message);
    printf("%ld", send(client_fd, message, strlen(message), 0));
}

void recive_message(int client_fd, int *connection_status) {
    char buffer[1024] = {0};
    int valread = read(client_fd, buffer, sizeof(buffer) - 1);

    if (valread == -1) {
        perror("Error reading from client");
    } else {
        buffer[valread] = '\0'; 
        printf("%s\n", buffer);
    }

}

int check_ip_exists(char *ip_address) {
    /*
        Ping PC by IP address
    */

    const char *scriptPath = "./check_ip.sh";
    char command[256];

    snprintf(command, sizeof(command), "%s %s", scriptPath, ip_address);
    int exitCode = system(command);

    if (exitCode == 0) {
        return 0;
    } else {
        return 1;
    }
}