#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <limits.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <stdarg.h>

#ifdef __linux__
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define SocketType int
#define INVALID_SOCKET -1
#define SD_SEND 1

#elif defined _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>

#define SocketType SOCKET

#pragma comment(lib, "Ws2_32.lib")
#endif

void error(const char* msg)
{
    perror(msg);
    exit(1);
}


/***************************
*  Platform dependent code *
****************************/
void initSocket()
{
#ifdef __linux__
    //No additional initialisation is needed 
#elif defined _WIN32
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
    {
        error("WSAStartup failed");
    }
#endif
}

void cleanup()
{
#ifdef __linux__
    // No extra cleanup is needed
#elif defined _WIN32
    WSACleanup();
#endif
}

void zeroMemory(void** data, size_t amount)
{
#ifdef __linux__
    bzero(*data, amount);
#elif defined _WIN32
    ZeroMemory(*data, amount);
#endif
}

void closeSocket(SocketType sockfd)
{
#ifdef __linux__
    close(sockfd);
#elif defined _WIN32
    closesocket(sockfd);
#endif
}

bool isSocketValid(SocketType sockfd)
{
#ifdef __linux__
    if (sockfd < 0)
#elif defined _WIN32
    if (sockfd == INVALID_SOCKET)
#endif
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool isSocketOK(int err)
{
#ifdef __linux__
    if (err < 0)
#elif defined _WIN32
    if (err == SOCKET_ERROR)
#endif
    {
        return false;
    }
    else
    {
        return true;
    }
}

void newstrcpy(char** dest, const char* source)
{
#ifdef __linux__
    strcpy(*dest, source);
#elif defined _WIN32
    strcpy_s(*dest, strlen(source), source);
#endif
}

void newstrcat(char** dest, const char* source)
{
#ifdef __linux__
    strcat(*dest, source);
#elif defined _WIN32
    strcat_s(*dest, strlen(source), source);
#endif
}

FILE* newfopen(const char* filePath, const char* mode)
{
    FILE* f;

#ifdef __linux__
    f = fopen(filePath, mode);
#elif defined _WIN32
    fopen_s(&f, filePath, mode);
#endif

    return f;
}

/****************************
* Platform independent code *
*****************************/
int parseConfigLine(const char* line, const char* param, const long min, const long max)
{
    int res = -1;

    size_t headerLen = strlen(param);
    size_t len = strlen(line) - headerLen;

    char* str = (char*)malloc(len + 1);
    if (str == NULL)
    {
        //error("Cannot allocate memory for port");
        return -2;
    }

    memset(str, '\0', len);
    memcpy(str, &line[headerLen], len);
    str[len] = '\0';

    errno = 0;
    char* end;
    const long num = strtol(str, &end, 10);
    free(str);

    if (errno == ERANGE)
    {
        //error("Range error occurred");
        return -3;
    }

    if (num <= max && num >= min && min > 0)
    {
        res = (int)num;
    }
    else
    {
        //error("Port is out of range");
        return -4;
    }

    return res;
}

char* parseConfigLineString(const char* line, const char* param)
{
    size_t headerLen = strlen(param);
    size_t len = strlen(line) - headerLen;
    char* result = (char*)malloc(len);
    if (result == NULL)
    {
        return NULL;
    }

    memcpy(result, &line[headerLen], len - 1);
    result[len] = '\0';

    return result;
}

int main(int argc, char* argv[])
{
    // Open config file
    FILE* cfg = newfopen("config.cfg", "r");
    if (cfg == NULL)
    {
        error("Cannot open config file");
    }

    const char PORT[] = "PORT: ";
    const char HOST[] = "HOST: ";
    const char BUFFER_LENGTH[] = "BUFFER_LENGTH: ";
    const long MAX_BUFFER_LENGTH = 1024;
    const int MAX_LINE_LENGTH = 100;

    int lineSize = 0;
    size_t buflen = 0;
    char* line = NULL,
        * host = NULL,
        * port = NULL;

    // Parse config file
    line = (char*)malloc(MAX_LINE_LENGTH);
    zeroMemory(&line, MAX_LINE_LENGTH);
    while (fgets(line, MAX_LINE_LENGTH - 1, cfg) != NULL)
    {
        if (strstr(line, PORT) != NULL)
        {
            port = parseConfigLineString(line, PORT);
            if (port == NULL)
            {
                if (host) free(host);
                if (line) free(line);
                fclose(cfg);
                error("Cannot parse port");
            }
        }
        else if (strstr(line, HOST) != NULL)
        {
            host = parseConfigLineString(line, HOST);

            if (host == NULL)
            {
                if (port) free(port);
                if (line) free(line);
                fclose(cfg);
                error("Cannot allocate memory for host");
            }
        }
        else if (strstr(line, BUFFER_LENGTH) != NULL)
        {
            int res = parseConfigLine(line, BUFFER_LENGTH, 1, MAX_BUFFER_LENGTH);

            if (res < 0)
            {
                if (port) free(port);
                if (host) free(host);
                if (line) free(line);
                fclose(cfg);
                error("Cannot parse buffer length");
            }
            else
            {
                buflen = res;
            }
        }
    }
    fclose(cfg);
    if (line) free(line);

    const char SECRET_KEY[] = "Test ";

    SocketType sockfd = INVALID_SOCKET;

    struct addrinfo* result = NULL,
        * ptr = NULL,
        * hints = NULL;

    initSocket();

    hints = (struct addrinfo*)malloc(sizeof(struct addrinfo));
    zeroMemory(&hints, sizeof(hints));
    hints->ai_family = AF_UNSPEC;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;

    int err = 0;
    // Resolve the server address and port
    err = getaddrinfo(host, port, hints, &result);
    free(hints);

    if (err != 0)
    {
        cleanup();
        error("getaddrinfo failed");
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        sockfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (!isSocketValid(sockfd))
        {
            freeaddrinfo(result);
            cleanup();
            error("opening socket failed");
        }

        // Connect to server.
        err = connect(sockfd, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (!isSocketOK(err))
        {
            closeSocket(sockfd);
            sockfd = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    char* buffer = (char*)malloc(buflen);
    char* header = (char*)malloc(2ULL * buflen);

    zeroMemory(&buffer, buflen);
	zeroMemory(&header, 2ULL * buflen);

    int blen = 0;
    if (buflen < INT_MAX)
    {
        blen = (int)buflen;
    }
    else
    {
        free(buffer);
        free(header);
        cleanup();
        error("header is too big");
    }

    printf("Please enter the names of players: ");
    fgets(buffer, blen - 1, stdin);
    newstrcpy(&header, SECRET_KEY);
    newstrcat(&header, buffer);

    int len = 0;
    size_t slen = strlen(header);
    if (slen < INT_MAX)
    {
        len = (int)slen;
    }
    else
    {
        free(buffer);
        free(header);
        cleanup();
        error("header is too big");
    }

    printf("%s", header);
    err = send(sockfd, header, len, 0);
    free(header);

    if (err < 0)
    {
        free(buffer);
        cleanup();
        error("sending to socket failure");
    }

    zeroMemory(&buffer, buflen);
    err = recv(sockfd, buffer, blen - 1, 0);
    if (err < 0)
    {
        free(buffer);
        cleanup();
        error("reading from socket failure");
    }
    printf("%s\n", buffer);
    free(buffer);

    closeSocket(sockfd);
    cleanup();
    return 0;
}