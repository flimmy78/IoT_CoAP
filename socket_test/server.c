#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PORT	7777

int main() {
	int server_sockfd, client_sockfd;
	int client_len;
	int pid;

	struct sockaddr_in6 clientaddr, serveraddr;

	char buf[255];

	// 소켓 생성
	if((server_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket error : ");
		exit(0);
	}

	memset(&serveraddr, 0x00, sizeof(serveraddr));
	serveraddr.sin6_family = AF_INET;
	serveraddr.sin6_addr = in6addr_any;
	serveraddr.sin6_port = htons(PORT);

	// 바인딩 (소켓에 이름 연결)
	if(bind(server_sockfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
		perror("bind error : "); 
		exit(0);
	}

	if(listen(server_sockfd, 5) == -1) {
		perror("listen error : ");
		exit(0);
	}

	while(1) {
		client_sockfd = accept(server_sockfd, (struct sockaddr*)&clientaddr, &client_len);

		if(client_sockfd == -1) {
			perror("Accept error : ");
			exit(0);
		}
		printf("a client connected\n");

		/* 출력문 넣어서 이 위치에서 하는 오퍼레이션 확인 */
		while(1) {
			/* rewind -> 파일 읽기/쓰기 포인터 위치를 처음으로 초기화 */
			memset(buf, '\0', 255);

			/* 클라이언트가 입력한 내용을 버퍼에 담는다 read는 fd에서 읽어들인 바이트수를 반환 */
			if(read(client_sockfd, buf, 255) <= 0) {
				close(client_sockfd);
				break;
			}
			write(client_sockfd, buf, 255);
			printf("Data: %s\n", buf);
		}
		printf("a client disconnected\n");
	}
	close(client_sockfd);
}