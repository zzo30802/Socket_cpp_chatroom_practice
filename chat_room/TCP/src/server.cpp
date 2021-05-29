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

#pragma region global_variable
#pragma endregion
/*
學習資源
Boost.Asio C++教學: https://mmoaay.gitbooks.io/boost-asio-cpp-network-programming-chinese/content/Chapter3.html
TCP Socket教學(socket function講解): http://zake7749.github.io/2015/03/17/SocketProgramming/
*/
DWORD WINAPI thread_receive_client_msg(LPVOID p_client_socket);

//  http://c.biancheng.net/cpp/html/3031.html
int main() {
  //-----load winsock2環境-----
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cout << "[Error] WSAStartup error: " << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----create socket-----
  // int socket(int domain, int type, int protocol);
  // domain : 指定協議使用PF_INET, 設定IP使用AF_INET, 但混用沒差(值是相同的)
  // type : SOCK_STREAM = TCP, SOCK_DGRAM = UDP
  // protocol : 一般使用0，讓kernel選擇type對應的默認協議。
  SOCKET server_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket == INVALID_SOCKET) {
    std::cout << "[Error]: " << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----bind socket-----
  // sockaddr_in : 儲存資料的struct
  int len{sizeof(sockaddr_in)};
  sockaddr_in socket_address;
  std::memset(&socket_address, 0, len);  // allocate memory & initialize to 0
  socket_address.sin_family = PF_INET;   // IPv4
  // inet_addr() : 將字串轉乘整數形式的IP
  socket_address.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP address
  // htons() : 網路端的字節序&本機端的字節序可能不一致。
  // 網路端總是用Big endian，而本機端要視系統而定，比如x86使用的是little endian。
  // 所以htons就是Host To Network Short integer的縮寫，他將本機端的字節序(endian)轉成
  // 了網路端的字節序。
  // htons(), htonl(), ntohl(), ntohs()
  socket_address.sin_port = htons(8080);
  // bind() : 將address綁在Socket上。
  if (bind(server_socket, (SOCKADDR*)&socket_address, sizeof(SOCKADDR)) == SOCKET_ERROR) {
    std::cout << "[Error]: bind() error" << std::endl
              << "\t" << WSAGetLastError() << std::endl;
    return -1;
  }

  //-----listen-----
  // listen將socket設置為被動監聽類型，等待client發送connect請求
  // IP:對應到一個城市，Port:城市的港口，bind()告訴別人我們住在哪裡，該從哪個港口登陸後，
  // 我們必須反覆去察看客人來了沒有，這個過程就是監聽(listen)，對應到Socket的listen()。
  // int listen(int sockfd, int backlog);
  // sockfd  : 傳入SOCKET的instance
  // backlog : 規定有多少個client可以連入server
  listen(server_socket, 20);

  std::cout << "Listening for incoming connections." << std::endl;
  //-----main thread loop接收client的request-----
  sockaddr_in client_address;
  std::memset(&client_address, 0, len);
  while (true) {
    //-----成功收到client返回的socket-----
    SOCKET client_socket = accept(server_socket, (SOCKADDR*)&client_address, &len);
    if (client_socket != INVALID_SOCKET) {
      //-----create thread to receive-----
      HANDLE thread = CreateThread(NULL, 0, thread_receive_client_msg, (LPVOID)client_socket, 0, NULL);
      CloseHandle(thread);
    }
  }
  //-----close socket-----
  closesocket(server_socket);
  // clear winsocket2的環境
  WSACleanup();
  return 0;
}

DWORD WINAPI thread_receive_client_msg(LPVOID p_client_socket) {
  //-----與client通訊-----
  // 傳入void pointer轉成SOCKET指標
  SOCKET client_socket = (SOCKET)p_client_socket;
  printf("Welcome %d into the chat room.\n", client_socket);
  // 發送數據
  char buf[100]{0};
  sprintf(buf, "Welcome the chat room.", client_socket);
  send(client_socket, buf, 100, 0);
  // loop接收客戶端數據
  int ret{0};
  do {
    char recv_msg[100]{0};
    ret = recv(client_socket, recv_msg, 100, 0);

    if (strcmp(recv_msg, (char*)("")) == 0) {
    } else
      std::cout << client_socket << " said: " << recv_msg << std::endl;
  } while (ret != SOCKET_ERROR && ret != 0);
  std::cout << client_socket << " Leaving the chat room." << std::endl;
  return 0;
}

#endif  // _SERVER_H_