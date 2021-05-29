#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <WinSock2.h>
#include <stdio.h>

#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

int main() {
  //-----load winsock2 environment-----
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    std::cout << "[Error] WSAStartup error: " << WSAGetLastError() << std::endl;
    return -1;
  }
  //-----create socket-----
  SOCKET &&client_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP);
  if (client_socket == INVALID_SOCKET) {
    std::cout << "[Error]: " << WSAGetLastError() << std::endl;
    return -1;
  }
  //-----bind socket-----
  int len{sizeof(sockaddr_in)};
  sockaddr_in client_address;
  std::memset(&client_address, 0, len);
  client_address.sin_family = PF_INET;
  client_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  client_address.sin_port = htons(8080);
  if (bind(client_socket, (SOCKADDR *)&client_address, len) < 0) {
    std::cout << "[Error]: " << WSAGetLastError() << std::endl;
    return -1;
  }

  char send_msg[100]{0};
  int ret{0};
  do {
    std::cout << "Please enter chat content: ";
    std::cin >> send_msg;
    if (strcmp(send_msg, (char *)("exit")) == 0) {
      std::cout << "Client Socket closed" << std::endl;
      return 0;
    } else {
      sendto(client_socket, send_msg, sizeof(send_msg) - 1, 0, (SOCKADDR *)&client_address, sizeof(SOCKADDR));
    }
  } while (1);

  //-----close socket-----
  closesocket(client_socket);
  //-----clear winsock2的環境-----
  WSACleanup();
  return 0;
}
#endif  // _CLIENT_H_