#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <WinSock2.h>
#include <stdio.h>

#include <iostream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

int main() {
  //-----load winsock2環境-----
  WSADATA wsad_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsad_data) != 0) {
    std::cout << "[Error] WSAStartup error: " << GetLastError() << std::endl;
    return -1;
  }
  //-----create socket-----
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (client_socket == INVALID_SOCKET) {
    std::cout << "[Error] socket error: " << GetLastError() << std::endl;
    return -1;
  }
  //-----connect to server-----
  int len{sizeof(sockaddr_in)};
  sockaddr_in socket_address;
  std::memset(&socket_address, 0, len);
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  socket_address.sin_port = htons(8080);
  if (connect(client_socket, (SOCKADDR *)&socket_address, len) == SOCKET_ERROR) {
    std::cout << "[Error] connect error: " << GetLastError() << std::endl;
    return -1;
  }

  //-----接收服務端的訊息-----
  char buf[100]{0};
  recv(client_socket, buf, 100, 0);
  std::cout << buf << std::endl;

  //-----隨時送訊息給server-----
  // char aaa[100]{0};
  char *aaa = "exit";
  // sprintf(aaa, "exit", client_socket);
  int ret{0};
  do {
    char send_msg[100]{0};
    std::cout << "Please enter chat content: ";
    std::cin >> send_msg;
    if (strcmp(send_msg, aaa) == 0) {
      std::cout << "Client Socket closed" << std::endl;
      return 0;
    } else
      ret = send(client_socket, send_msg, 100, 0);
  } while (ret != SOCKET_ERROR && ret != 0);

  //-----close socket-----
  closesocket(client_socket);
  //-----clear winsock2的環境-----
  WSACleanup();
  return 0;
}

#endif  // _CLIENT_H_