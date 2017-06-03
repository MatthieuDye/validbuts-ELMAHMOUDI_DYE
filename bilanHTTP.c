// http_client.c
// par Chloé DALGER et Matthieu DYE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int sock;

void Error(char *mess)
{
  fprintf(stderr,"%s\n",mess);
  if (sock) close(sock);
  exit(-1);
}

int main(void) {

/*
à envoyer à IGFAR_PartieX

type_mess      type_ent          num              data
   IP		  VB                          (adresse ip)
*/
  /* Saisie des infos de connexion */
  char *host="www.dweet.io";
  char file[100]="/dweet/for/robot18?";
  int port=80;


  /* Calcul de l'IP de l'hote */
  struct hostent *hostinfo;
  hostinfo = gethostbyname(host); 
  if (hostinfo) {
    struct in_addr  **pptr;
    pptr = (struct in_addr **)hostinfo->h_addr_list;
    printf("Adresse IP de l'hote : %s\n",inet_ntoa(**(pptr)));
  } 

  

int cpt =0;
int x;
int y;
char xStr[5];
char yStr[5];
while (cpt < 50) { 
/* Creation de la socket */
  if ( (sock = socket(AF_INET, SOCK_STREAM, 0)) <0 )
    Error("can't create socket");

  x = rand() % 300 ;
  y = rand() % 300 ;
  
  strcat(file,"x=");

  xStr[0] = '\0';
  sprintf(xStr,"%d",x);
  yStr[0] = '\0';
  sprintf(yStr,"%d",y);


    strcat(file,xStr);
    strcat(file,"&y=");
    strcat(file,yStr);
    /* Configuration de la connexion */

  struct sockaddr_in sin;
 
  sin.sin_addr = *(struct in_addr *) hostinfo->h_addr;
  sin.sin_family = AF_INET;
  if ( (sin.sin_port = htons(port)) == 0)
    Error("unknown service");

  /* Tentative de connexion au serveur */
  if (connect(sock, (struct sockaddr*)&sin,sizeof(sin))<0)
    Error("can't connect");
  else printf("Connexion a %s sur le port %d\n", inet_ntoa(sin.sin_addr),
         htons(sin.sin_port));

  /* Envoi de donnees au serveur */
  char buffer[1024] = "GET ";
  strcat(buffer, file);
  strcat(buffer, " HTTP/1.1\nHost: ");
  strcat(buffer, host);
  strcat(buffer, "\n\n");
  printf("Requete : \n%s", buffer);

  // TO DO : test whether this suceeds or Erorr("write error on socket")
  send(sock, buffer, 1024, 0);

  /* Reception de donnees du serveur */
  char buffer2[1024] = "";
  for(;;) {
    int res = recv(sock, buffer2, sizeof buffer2 - 1, 0);
    if (res > 0) {
      buffer2[res] = '\0';
      printf("%s", buffer2);
      fflush(stdout);
    }//if
    else
      break;
  }//

  strcpy(file,"/dweet/for/robot18?");
  sleep(2);
  /* Fermeture de la socket client */
  close(sock);
  }
  return EXIT_SUCCESS;
 
}//main
