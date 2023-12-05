#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  // Create a socket
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
  {
    std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    return -1;
  }

  // Bind the socket to the BeagleBone's IP address and port 8081
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8081);                         // Port number
  inet_pton(AF_INET, "192.168.1.103", &serverAddress.sin_addr); // BeagleBone's IP address

  if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
  {
    std::cerr << "Error binding socket: " << strerror(errno) << std::endl;
    close(serverSocket);
    return -1;
  }

  // Listen for incoming connections
  if (listen(serverSocket, 10) == -1)
  {
    std::cerr << "Error listening on socket: " << strerror(errno) << std::endl;
    close(serverSocket);
    return -1;
  }

  std::cout << "Server listening on 192.168.1.103:8081...\n";

  // Accept a connection
  sockaddr_in clientAddress;
  socklen_t clientAddrSize = sizeof(clientAddress);
  int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddrSize);

  if (clientSocket == -1)
  {
    std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
    close(serverSocket);
    return -1;
  }

  std::cout << "Connection accepted from " << inet_ntoa(clientAddress.sin_addr) << ":" << ntohs(clientAddress.sin_port) << std::endl;

  // Send "Hello, World!" every second
  const char *message = "Hello, World!";
  while (true)
  {
    send(clientSocket, message, strlen(message), 0);
    sleep(1); // Wait for 1 second
  }

  // Close the sockets (this part is not reached in the example)
  close(clientSocket);
  close(serverSocket);

  return 0;
}
