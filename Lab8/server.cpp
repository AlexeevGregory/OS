#include <iostream>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>


using namespace std;


pthread_mutex_t m;
pthread_t id_ar, id_sr, id_wfc;
int sock;
int listener;
struct sockaddr_in addr;
vector <string> msglist;
int flag_ar = 0, flag_sr = 0, flag_wfc = 0;


void *accepting_requests(void *arg){
	char buf[256];
	while(flag_ar == 0){
		int bytes_received = recv(sock, buf, sizeof(buf), 0);
		if (bytes_received == -1) {
			perror("recv");
			break;
		}
		else{
			pthread_mutex_lock(&m);
			msglist.push_back(buf);
			pthread_mutex_unlock(&m);
			printf("\nresult of request = %s\n", buf);
			//polozhit v queue zapros (queue - own resource)
			//nuzhno takzhe zahvat mutex
		}
	}
	printf("\nthread accepting_requests has finished working\n");
	pthread_exit((void*)2);
}
void *sending_response(void *arg){
	clock_t time;
	char buf[256];
	while(flag_sr == 0){
		//buf = msglist.back();
		pthread_mutex_lock(&m);
		if(!msglist.empty()) {
			msglist.pop_back();
			pthread_mutex_unlock(&m);
			time = clock();
			sprintf(buf, "%ld", time);
			send(sock, buf, sizeof(buf), 0);
		}
		else {
			pthread_mutex_unlock(&m);
			sleep(1);
		}
		
		
	}
	printf("\nthread sending_response has finished working\n");
	pthread_exit((void*)2);
}
void *waiting_for_connections(void *arg){
	while(flag_wfc == 0){
		sock = accept(listener, NULL, NULL);
		if (sock == -1) {
			perror("accept");
			break;
		}
		else{
			pthread_create(&id_ar, NULL, accepting_requests, NULL);
			pthread_create(&id_sr, NULL, sending_response, NULL);
			break;
		}
	}
	printf("\nthread waiting_for_connections has finished working\n");
	pthread_exit((void*)2);
}


int main() {
	pthread_mutex_init(&m, NULL);
	int optval = 1;
	
	listener = socket(AF_INET, SOCK_STREAM, 0);
	
	if (listener == -1) {
		perror("socket");
        	return 1;
	}
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7000);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");



	if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
		perror("bind");
		return 1;
	}
	
	if (setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
		perror("setsockopt");
		return 1;
	}
	
	
	if (listen(listener, 1) == -1) {
		perror("listen");
		return 1;
	}


	pthread_create(&id_wfc, NULL, waiting_for_connections, NULL);
	
	printf("\nthe program is waiting to key pressed\n");
	getchar();
	printf("\nthe key is pressed\n");
	
	flag_ar = 1, flag_sr = 1, flag_wfc = 1;
	
	
	pthread_join(id_ar, NULL);
	pthread_join(id_sr, NULL);
	pthread_join(id_wfc, NULL);
	


	//shutdown(sock);
	close(listener);
	close(sock);
	
	pthread_mutex_destroy(&m);
	return 0;
}
