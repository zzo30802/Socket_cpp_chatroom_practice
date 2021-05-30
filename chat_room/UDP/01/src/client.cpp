#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <WinSock2.h>
#include <stdio.h>

#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")
/*
int main() {
  //-----load winsock2 environment-----
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
    std::cout << "[Error] WSAStartup error: " << WSAGetLastError() << std::endl;
    return -1;
  }
  //-----create socket-----
  SOCKET client_socket = socket(PF_INET, SOCK_DGRAM, 0);
  if (client_socket == INVALID_SOCKET) {
    std::cout << "[Error]: socket" << WSAGetLastError() << std::endl;
    return -1;
  }
  //-----bind socket-----
  int len{sizeof(sockaddr_in)};
  sockaddr_in client_address;
  std::memset(&client_address, 0, len);
  client_address.sin_family = AF_INET;
  client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  client_address.sin_port = htons(8080);
  // if (bind(client_socket, (SOCKADDR *)&client_address, len) < 0) {
  //   std::cout << "[Error]: bind" << WSAGetLastError() << std::endl;
  //   return -1;
  // }

  char send_msg[100];
  int ret{0};
  do {
    std::cout << "Please enter chat content: ";
    std::cin >> send_msg;
    if (strcmp(send_msg, (char *)("exit")) == 0) {
      std::cout << "Client Socket closed" << std::endl;
      return 0;
    } else {
      int ret = sendto(client_socket, send_msg, sizeof(send_msg), 0, (SOCKADDR *)&client_address, sizeof(SOCKADDR));
      if (ret == SOCKET_ERROR) {
        std::cout << "[Error]: sendto" << std::endl;
      }
    }
  } while (true);

  //-----close socket-----
  closesocket(client_socket);
  //-----clear winsock2的環境-----
  WSACleanup();
  return 0;
}
*/
#define BUF_SIZE 100
int main() {
  // load winsock2 environment
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    std::cout << "[Error] WSAStartup error: " << WSAGetLastError() << std::endl;
    return -1;
  }

  // create socket
  SOCKET client_sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  // client address info
  sockaddr_in server_address;
  std::memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = PF_INET;
  server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  server_address.sin_port = htons(8080);

  // input the message and send it to server
  SOCKADDR client_address;
  int &&sockarr_size = sizeof(SOCKADDR);
  while (1) {
    char buf[BUF_SIZE]{0};
    std::cout << "Please enter chat content: ";
    std::cin >> buf;
    if (strcmp(buf, (char *)("exit")) == 0) {
      char exit_str[BUF_SIZE]{0};
      sprintf(exit_str, "The client: %d leaving the char room.", client_sockfd);
      sendto(client_sockfd, exit_str, BUF_SIZE, 0, (SOCKADDR *)&server_address, sockarr_size);
      std::cout << "Client Socket closed" << std::endl;
      return 0;
    } else
      sendto(client_sockfd, buf, BUF_SIZE, 0, (SOCKADDR *)&server_address, sockarr_size);
  }
  //-----close socket-----
  closesocket(client_sockfd);
  //-----clear winsock2的環境-----
  WSACleanup();
  return 0;
}
#endif  // _CLIENT_H_