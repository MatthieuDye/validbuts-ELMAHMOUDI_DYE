// tcp_serveur.c

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#define PORT 10000

// Pour les variables globales suvantes, les valeurs sont codée en brut, et de manière arbitraire
// A vous de les renseigner en fonction de votre réponse

char adrRobot [15] = "162.38.111.103"; // adresse ip du robot
char idBallon [15] = "1"; // reference du ballon qui vous a été donné
char coulRobot[15] = "rouge" ;// ou "bleu";
int penalty = 0;

void proc_exit(int sig) {
  //wait();
}

int main(void) {

  /* Pour eviter les zombies */
  signal(SIGCHLD, proc_exit);

  /* Socket et contexte d'adressage du serveur */
  struct sockaddr_in sin;
  int sock;
  socklen_t recsize = sizeof(sin);
    
  /* Socket et contexte d'adressage du client */
  struct sockaddr_in csin;
  int csock;
  socklen_t crecsize = sizeof(csin);

  /* Pour forker le moment venu */
  pid_t pid;
 
  /* Creation d'une socket */
  sock = socket(AF_INET, SOCK_STREAM, 0);

  /* Configuration */
  sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
  sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
  sin.sin_port = htons(PORT);               /* Listage du port */
  bind(sock, (struct sockaddr*)&sin, recsize);

  /* Demarrage du listage (mode server) */
  listen(sock, 5);

  for(;;) {
    /* Attente d'une connexion client */
    csock = accept(sock, (struct sockaddr*)&csin, &crecsize);
    printf("Le validateur de but fait une demande !\nIl se connecte avec la socket %i de %s:%d\n",
           csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));

    pid = fork();

    if (pid == 0) {
      /* Envoi de donnees au client */
      char buffer[50] = "";
      strcat(buffer,adrRobot);
      strcat(buffer,"/");
      strcat(buffer,idBallon);
      strcat(buffer,"/");
      strcat(buffer,coulRobot);
	    strcat(buffer,"/");
      send(csock, buffer,50 , 0);
	    printf("Envoi de %s\n", buffer);

    
        /* Reception de donnees du client */
        int res = recv(csock, buffer, 32, 0);
        if (res == 0)
          exit(0);
        printf("Recu de la socket %d de %s:%d : %s\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port), buffer);

        if (strcmp(buffer,"1")==0) {
            printf("Je viens de marquer un but.\n");
            }

        else if (strcmp(buffer,"0")==0) {
            printf("Mon ballon n'existe pas.\n");
            penalty = 1;
            }

        else if (strcmp(buffer,"-1")==0) {
            printf("Je viens de marquer contre mon camp.\n");
            }

        else if (strcmp(buffer,"J")==0) {
            printf("Mon propriétaire n'est pas référencé.\n");
            penalty = 2;
            }

        else {
          printf("Occurence d'une erreur non gérée\n");
        }



        if (penalty!=0) {
          printf("Suite à une faute de votre part, vous avez une pénalité de %i minutes. \n", penalty);
        }

    

      /* Fermeture de la socket dans les deux sens */
      shutdown(csock, 2);

      /* Fermeture de la socket client */
      close(csock);
    }//if
  }//for

  /* Fermeture de la socket serveur */
  close(sock);

  return EXIT_SUCCESS;

}//main
