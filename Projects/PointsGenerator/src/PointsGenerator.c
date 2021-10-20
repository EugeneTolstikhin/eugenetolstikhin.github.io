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

/***************************
*  Platform dependent code *
****************************/
void cleanup()
{
#ifdef __linux__
    // No extra cleanup is needed
#elif defined _WIN32
    WSACleanup();
#endif
}

void newsprintf(char** str, const char* format, ...)
{
    va_list args;
    va_start(args, format);

#ifdef __linux__
    vsprintf(*str, format, args);
#elif defined _WIN32
    int len = _vscprintf(format, args)  // _vscprintf doesn't count
        + 1; // terminating '\0'
    vsprintf_s(*str, len, format, args);
#endif

    va_end(args);
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

void setupSignalInterception(int sigName, void(*callBack)(int))
{
#ifdef __linux__
    struct sigaction act;
    memset(&act, '\0', sizeof(act));
    act.sa_handler = callBack;
    sigaction(sigName, &act, NULL);
#elif defined _WIN32
    signal(sigName, callBack);
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

/****************************
* Platform independent code *
*****************************/
static bool keepRunning = true;

static void intHandler(int dummy)
{
    keepRunning = false;
}

void error(const char* msg)
{
    perror(msg);
    exit(1);
}

int parseConfigLine(const char* line, const char* param, long min, long max)
{
    int res = -1;

    size_t headerLen = strlen(param);
    size_t len = strlen(line) - headerLen;

    char* str = (char*)malloc(len + 1ULL);
    if (str == NULL)
    {
        //error("Cannot allocate memory for port");
        return -2;
    }

    memset(str, '\0', len);
    memcpy(str, &line[strlen(param)], len);
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

int main(int argc, char* argv[])
{
    setupSignalInterception(SIGINT, intHandler);
    srand((unsigned int)time(NULL));

    // Open config file
    FILE* cfg = newfopen("config.cfg", "r");
    if (cfg == NULL)
    {
        error("Cannot open config file");
    }

    int port = 0;
    int lineSize = 0;
    size_t buflen = 0;
    char* line = NULL, * host = NULL;

    const char PORT[] = "PORT: ";
    const char BUFFER_LENGTH[] = "BUFFER_LENGTH: ";
    const long MAX_BUFFER_LENGTH = 1024;
    const int POOL_SIZE = 5;
    const int RAND_MAXIMUM = 11;
    const int MAX_LINE_LENGTH = 100;

    // Parse config file
    line = (char*)malloc(MAX_LINE_LENGTH);
    memset(line, '\0', MAX_LINE_LENGTH);
    while (fgets(line, MAX_LINE_LENGTH - 1, cfg) != NULL)
    {
        if (strstr(line, PORT) != NULL)
        {
            port = parseConfigLine(line, PORT, 1, USHRT_MAX);

            if (port < 0)
            {
                if (line) free(line);
                fclose(cfg);
                error("Cannot parse port");
            }
        }
        else if (strstr(line, BUFFER_LENGTH) != NULL)
        {
            int res = parseConfigLine(line, BUFFER_LENGTH, 1, MAX_BUFFER_LENGTH);

            if (res < 0 || buflen > INT_MAX)
            {
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

    int err = 0;
	SocketType sockfd = INVALID_SOCKET;

#if defined _WIN32
    WSADATA wsaData;
    err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
    {
        error("WSAStartup failed");
    }
#endif

    struct addrinfo* result = NULL;
    struct addrinfo* hints = (struct addrinfo*)malloc(sizeof(struct addrinfo));

    zeroMemory(&hints, sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM;
    hints->ai_protocol = IPPROTO_TCP;
    hints->ai_flags = AI_PASSIVE;

    const unsigned int PORT_LENGTH = 6;
    char* portStr = (char*)malloc(PORT_LENGTH);   // Max port value = USHRT_MAX (65535)
    zeroMemory(&portStr, PORT_LENGTH);
    newsprintf(&portStr, "%d", port);
    // Resolve the server address and port
    err = getaddrinfo(NULL, portStr, hints, &result);
    free(hints);
    free(portStr);

    if (err != 0)
    {
        cleanup();
        error("getaddrinfo failed");
    }

    sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (!isSocketValid(sockfd))
    {
        freeaddrinfo(result);
        cleanup();
        error("opening socket failed");
    }

    err = bind(sockfd, result->ai_addr, (int)result->ai_addrlen);
	if (!isSocketOK(err))
    {
        freeaddrinfo(result);
		closeSocket(sockfd);
        cleanup();
        error("binding socket failed");
    }

    freeaddrinfo(result);

    err = listen(sockfd, POOL_SIZE);
	if (!isSocketOK(err))
    {
		closeSocket(sockfd);
        cleanup();
        error("listening failed");
    }

    const char SECRET_KEY[] = "Give me the points!";
    const char SECRET_KEY_ANSWER[] = "I give you: ";
    const char READY_MESSAGE[] = "Wait for command!";

	SocketType newsockfd = INVALID_SOCKET;
    char* buffer = (char*)malloc(buflen);
    while ((newsockfd = accept(sockfd, NULL, NULL)) >= 0)
    {
        int len = 0;
        size_t slen = strlen(READY_MESSAGE);
        if (slen < INT_MAX)
        {
            len = (int)slen;
        }
        else
        {
            error("Ready message is too long");
        }
        send(newsockfd, READY_MESSAGE, len, 0);
        zeroMemory(&buffer, buflen);

        err = recv(newsockfd, buffer, (int)buflen - 1, 0);

        if (err < 0)
        {
            free(buffer);
			closeSocket(newsockfd);
			closeSocket(sockfd);
            error("reading from socket failed");
        }
        else if (strstr(buffer, SECRET_KEY) != NULL)
        {
            printf("Buffer: %s\n", buffer);
            fflush(stdin);
            int r = rand() % RAND_MAXIMUM;

            len = 0;
            slen = strlen(SECRET_KEY_ANSWER);
            if (slen < INT_MAX)
            {
                len = (int)slen;
            }
            else
            {
                error("Secret key answer is too long");
            }

            char* answer = (char*)malloc(slen + 2ULL);
            newsprintf(&answer, "%s%d", SECRET_KEY_ANSWER, r);
            printf("Answer: %s\n", answer);
            fflush(stdin);
            send(newsockfd, answer, len + 2, 0);
            free(answer);
        }

		closeSocket(newsockfd);
        if (!keepRunning) break;
    }

    free(buffer);

    // shutdown the connection since we're done
    err = shutdown(sockfd, SD_SEND);
	if (!isSocketOK(err))
    {
		closeSocket(sockfd);
        cleanup();
        error("shutdown failed");
    }

	closeSocket(sockfd);
    cleanup();
    return 0;
}
