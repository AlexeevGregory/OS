#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string.h>

using namespace std;

pthread_t id_sr, id_ar, id_wfc;
int sock;
struct sockaddr_in addr;
int flag_sr = 0, flag_ar = 0, flag_wfc = 0;

void* sending_requests(void *arg){
	char buffer[] = "request";
	while(flag_sr == 0){
		if (send(sock, buffer, sizeof(buffer), 0) == -1){
			perror("send");
			break;
		}
		printf("\nsending buffer: %s\n", buffer);
		sleep(1);
	}
	printf("\nthread sending_requests has finished working\n");
	pthread_exit((void*)2);
}
void* accepting_requests(void* arg){
	char buf[256];
	while(flag_ar == 0){
		recv(sock, buf, sizeof(buf), 0);
		printf("\naccepting buffer: %s\n", buf);
		sleep(1);
	}
	printf("\nthread accepting_requests has finished working\n");
	pthread_exit((void*)2);
}
void *waiting_for_connections(void *arg){
	while(flag_wfc == 0){
		if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
			perror("connect");
			break;
		}
		else{
			printf("\nconnection is established\n");
			pthread_create(&id_sr, NULL, sending_requests, NULL);
			pthread_create(&id_ar, NULL, accepting_requests, NULL);
			break;
		}
	}
	printf("\nthread waiting_for_connections has finished working\n");
	pthread_exit((void*)2);
}



int main() {
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		perror("socket");
		return 1;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	pthread_create(&id_wfc, NULL, waiting_for_connections, NULL);

	
	printf("\nthe program is waiting to key pressed\n");
	getchar();
	printf("\nthe key is pressed\n");
	
	flag_ar = 1, flag_sr = 1, flag_wfc = 1;
	
	
	pthread_join(id_ar, NULL);
	pthread_join(id_sr, NULL);
	pthread_join(id_wfc, NULL);
	

	close(sock);

	return 0;
}
