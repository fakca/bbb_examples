#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  // Create a socket
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSocket == -1)
  {
    std::cerr << "Error creating socket: " << strerror(errno) << std::endl;
    return -1;
  }

  // Connect to the BeagleBone server
  sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = htons(8081);                         // Port number
  inet_pton(AF_INET, "192.168.1.103", &serverAddress.sin_addr); // BeagleBone's IP address

  if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
  {
    std::cerr << "Error connecting to server: " << strerror(errno) << std::endl;
    close(clientSocket);
    return -1;
  }

  std::cout << "Connected to the server on 192.168.1.103:8081\n";

  // Receive and print messages continuously
  char buffer[1024] = {0};
  while (true)
  {
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead == -1)
    {
      std::cerr << "Error receiving message: " << strerror(errno) << std::endl;
      break;
    }
    else if (bytesRead == 0)
    {
      // Connection closed by the server
      std::cout << "Server closed the connection." << std::endl;
      break;
    }

    std::cout << "Server says: " << buffer << std::endl;

    // Clear the buffer for the next message
    memset(buffer, 0, sizeof(buffer));
  }

  // Close the socket
  close(clientSocket);

  return 0;
}
