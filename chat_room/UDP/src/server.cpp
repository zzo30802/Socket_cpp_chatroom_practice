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
// https://blog.csdn.net/CSDN_WHB/article/details/89187866
// https://www.itread01.com/content/1550247328.html
DWORD WINAPI thread_receive_client_msg(LPVOID p_client_socket, sockaddr_in &client_addr);

int main() {
  //-----load winsock2 environment-----
  WSADATA wsa_data;
  if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
    std::cout << "[Error] WSAStartup error: " << WSAGetLastError() << std::endl;
    return -1;
  }
  //-----create socket-----
  SOCKET &&server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_UDP);
  if (server_socket == INVALID_SOCKET) {
    std::cout << "[Error]: " << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----bind socket-----
  int len{sizeof(sockaddr_in)};
  sockaddr_in socket_address;
  std::memset(&socket_address, 0, len);
  socket_address.sin_family = PF_INET;  // IPv4
  socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");
  socket_address.sin_port = htons(8080);
  if (bind(server_socket, (SOCKADDR *)&socket_address, len) < 0) {
    std::cout << "[Error]: " << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----listen-----
  // UDP不需要listen & accept
  // listen(server_socket, 20);

  //-----loop to listen the clients to connect to the server-----
  // sockaddr_in client_address;
  // std::memset(&client_address, 0, len);
  // std::cout << "Listening for incoming connections." << std::endl;
  // char recv_msg_from_client[100]{0};
  // char temp_msg_buf[100]{0};
  // while (true) {
  //   // UDP: server don't need to accept client
  //   recvfrom(server_socket, recv_msg_from_client, 100, 0, (SOCKADDR *)&client_address, (int *)sizeof(SOCKADDR));
  //   // if socket server receives "exit" from the client and then closes the thread to stop receive msg from the client.
  //   if ()
  // }

  // 迴圈處理訊息讀取傳送到客戶端
  sockaddr_in server_address;
  std::memset(&server_address, 0, len);
  int len{0};
  char recv_msg_from_client[100];
  while ((len = recvfrom(server_socket, recv_msg_from_client, sizeof(recv_msg_from_client) - 1, 0, (SOCKADDR *)&server_address, (int *)sizeof(SOCKADDR))) > 0) {
    recv_msg_from_client[len] = '\0';  // 在收到訊息最後面加個\0
    // 將收到的訊息列印出來
    printf("read [%s:%d]: %s\n", inet_ntoa(server_address.sin_addr), ntohs(server_address.sin_port), recv_msg_from_client);
  }
  // close socket
  closesocket(server_socket);
  // clear the environment of winsock2
  WSACleanup();
}

// DWORD WINAPI thread_receive_client_msg(LPVOID p_client_socket, sockaddr_in &client_addr) {
//   // initialize array of char
//   char recv_msg_from_client[100]{0};
//   char temp_msg_buf[100]{0};
//   // char send_msg_to_client[100]{0};
//   // receive data from client
//   SOCKET client_socket = (SOCKET)p_client_socket;
//   printf("Welcome %d into the char room.\n", client_socket);
//   while (1) {
//     recvfrom(client_socket, recv_msg_from_client, 100, 0, (SOCKADDR *)&client_addr, (int *)sizeof(SOCKADDR));
//     // if socket server receives "exit" from the client and then closes the thread to stop receive msg from the client.
//     if (strcmp(recv_msg_from_client, (char *)("exit")) == 0) {
//       sendto(client_socket, "Client Socket closed.", strlen("Client Socket closed.") + 1, 0, (SOCKADDR *)&client_addr, (int)sizeof(SOCKADDR));
//       std::cout << client_socket << " leaving the chat room." << std::endl;
//       break;  // 一旦client離開server就關掉這個thread
//     }
//     // print the message from client
//     sprintf(temp_msg_buf, "%s say: %s", inet_ntoa(client_addr.sin_addr), recv_msg_from_client);
//     printf("%s\n", temp_msg_buf);
//   }
//   // close socket
//   closesocket(client_socket);
//   // clear the environment of winsock2
//   WSACleanup();
// }
#endif  // _SERVER_H_