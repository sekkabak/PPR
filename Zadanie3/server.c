#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int error(int ern, const char *msg)
{
	perror(msg);
	return ern;
}

int main(void)
{
	int port = 12345;

	// zmienne niezainicjowane
	int fd;
	char buffer[256];
	socklen_t socklen;

	struct sockaddr_in serv_addr, cli_addr;

	// tworzymy gniazdo
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		return error(1, "socket()");

	// zapelniamy strukture zerami
	bzero((char *)&serv_addr, sizeof(serv_addr));

	// typ gniazda
	serv_addr.sin_family = AF_INET;
	// oczekujemy polaczen na kazdym adresie
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	// port, na ktorym nasluchujemy
	serv_addr.sin_port = htons(port);

	// mapujemy gniazdo
	if (bind(fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		return error(2, "bind()");

	// rozpoczynamy nasluchiwanie na gniezdzie
	listen(fd, 5);

	// kod obslugujacy nowe polaczenia
	socklen = sizeof(cli_addr);

	while (1)
	{
		bzero(buffer, 256);
		recvfrom(fd, (char *)buffer, 255,
					 MSG_WAITALL, (struct sockaddr *)&cli_addr,
					 &socklen);
		printf("Address: %s Port: %d\n", inet_ntoa(cli_addr.sin_addr), cli_addr.sin_port);

		// przepisanie początka buffera jako nazwa procesu
		char name[8];
		int i = 0;
		char newBuffer[256];
		while (buffer[i] != '\n')
		{
			name[i] = buffer[i];
			i++;
		}
		name[i] = '\0';
		i++;

		pid_t pid = fork();
		if (pid == 0)
		{
			// przepisanie reszty bufora do newBuffer
			int ii = 0;
			while (buffer[i] != '\0')
			{
				newBuffer[ii] = buffer[i];
				ii++;
				i++;
			}
			newBuffer[ii] = '\0';

			// zapis do pliku
			FILE *f = fopen(name, "a");
			if (f == NULL)
			{
				printf("Error opening file!\n");
				exit(1);
			}

			fprintf(f, "Wiadomosc od klienta to: %s\n", newBuffer);
			fprintf(f, "Wiadomosc w hexa: ");
			i = 0;
			while (newBuffer[i] != '\0')
			{
				fprintf(f, "%02X ", newBuffer[i]);
				i++;
			}
			fprintf(f, "\n");
			fclose(f);

			return 0;
		}
		else
		{
			printf("Powołuję podproces o id: %d\n\n", pid);
		}
	}
	
	close(fd);
	return 0;
}
