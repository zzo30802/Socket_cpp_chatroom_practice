#ifndef _SERVER_H_
#define _SERVER_H_
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#pragma comment(lib, "ws2_32.lib");  // ws2_32.dll
/*
int main() {
  //-----load winsock2 environment-----
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 0), &wsa_data) != 0) {
    std::cout << "[Error] WSAStartup error: " << WSAGetLastError() << std::endl;
    return -1;
  }
  //-----create socket-----
  SOCKET server_socket = socket(AF_INET, SOCK_DGRAM, 0);
  if (server_socket == INVALID_SOCKET) {
    std::cout << "[Error]: socket " << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----bind socket-----
  int len{sizeof(sockaddr_in)};
  sockaddr_in server_address;
  std::memset(&server_address, 0, len);
  server_address.sin_family = AF_INET;  // IPv4
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080);
  if (bind(server_socket, (SOCKADDR *)&server_address, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    std::cout << "[Error]: bind " << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----listen-----
  // UDP不需要listen & accept
  // listen(server_socket, 20);

  // 迴圈處理訊息讀取傳送到客戶端
  // int recv_msg_len;
  sockaddr_in client_address;
  char recv_msg_from_client[100]{0};
  // std::cout << "sizerecv_msg_from_client: " << <<
  while (true) {
    // std::memset(recv_msg_from_client, 0, sizeof(recv_msg_from_client));
    int recv_msg_len = recvfrom(server_socket, recv_msg_from_client, sizeof(recv_msg_from_client), 0, (SOCKADDR *)&client_address, (int *)sizeof(client_address));
    // std::cout << "sizeof(recv_msg_from_client): " << sizeof(recv_msg_from_client) << std::endl;
    // printf("AAAAAAA: %s", recv_msg_from_client);
    // std::cout << server_socket << " say: " << recv_msg_from_client << std::endl;
    if (recv_msg_len == SOCKET_ERROR) {
      // std::cout << "len : " << recv_msg_len << std::endl;
      continue;
    } else {
      std::cout << "len : " << recv_msg_len << std::endl;
      // recv_msg_from_client[recv_msg_len] = '\0';  // 在收到訊息最後面加個\0
      // 將收到的訊息列印出來
      printf("read [%s:%d]: %s\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port), recv_msg_from_client);
    }
  }
  // close socket
  closesocket(server_socket);
  // clear the environment of winsock2
  WSACleanup();
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
  SOCKET server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  // bind socket
  sockaddr_in server_address;
  std::memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = PF_INET;                 // IPv4 address
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);  // auto to receive IP address
  server_address.sin_port = htons(8080);
  bind(server_sockfd, (SOCKADDR *)&server_address, sizeof(SOCKADDR));

  // receive the message from client
  SOCKADDR client_address;
  char buf[BUF_SIZE];
  int &&sockarr_size = sizeof(SOCKADDR);
  while (1) {
    int &&str_len = recvfrom(server_sockfd, buf, BUF_SIZE, 0, &client_address, &sockarr_size);
    if (str_len == SOCKET_ERROR)
      continue;
    else {
      std::cout << client_address.sa_family << " say: " << buf << std::endl;
    }
    // sendto(server_sockfd, buf, str_len, 0, &client_address, buf_size);
  }
  closesocket(server_sockfd);
  WSACleanup();
  return 0;
}
#endif  // _SERVER_H_