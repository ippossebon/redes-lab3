#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define	SOCKET	int
#define INVALID_SOCKET  ((SOCKET)~0)

#define PORTA_CLI 2345
#define PORTA_SRV 2023
#define STR_IPSERVIDOR "143.54.6.12"
//#define STR_IPSERVIDOR "192.168.0.146"

int main(int argc, char* argv[])
{
  SOCKET s;
  struct sockaddr_in  s_cli, s_serv;

  /* Abre socket TCP, indicado por SOCK_STREAM */
  if ((s = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET)
  {
    printf("Erro iniciando socket\n");
    return(0);
  }

/*
  // seta informacoes IP/Porta locais
  s_cli.sin_family = AF_INET;
  s_cli.sin_addr.s_addr = htonl(INADDR_ANY);
  s_cli.sin_port = htons(PORTA_CLI);

  // associa configuracoes locais com socket
  if ((bind(s, (struct sockaddr *)&s_cli, sizeof(s_cli))) != 0)
  {
    printf("erro no bind\n");
    close(s);
    return(0);
  }
*/
  /* Seta informacoes IP/Porta do servidor remoto */
  s_serv.sin_family = AF_INET;
  s_serv.sin_addr.s_addr = inet_addr(STR_IPSERVIDOR);

  /* Recebe a porta por parâmetro do usuário. */
  int port = atoi(argv[1]);
  s_serv.sin_port = htons(port);

  // Conecta socket aberto no cliente com o servidor
  if(connect(s, (struct sockaddr*)&s_serv, sizeof(s_serv)) != 0)
  {
    printf("Erro na conexao");
    close(s);
    exit(1);
  }

#if 0
  // envia mensagem de conexao - aprimorar para dar IP e porta
  if ((send(s, "Conectado\n", 11,0)) == SOCKET_ERROR);
  {
    printf("erro na transmiss�o - %d\n", WSAGetLastError());
    closesocket(s);
    return 0;
  }
#endif

  /* Envia pacotes de 1250 bytes para o servidor */
  char str[1250];
  char ch;
  int i;

  /* Variáveis utilizadas para medir o tempo de envio de cada pacote. */
  int packages = 0;
  time_t initial_time, final_time;
  time(&initial_time);

  int t = 0;
  while(1)
  {
    str[0] = 'a';

    if ((send(s, (const char *)&str, sizeof(str),0)) < 0)
    {
      printf("Erro na transmissão\n");
      close(s);
      return 0;
    }
    if(strcmp((const char *)&str, "q")==0)
      break;

    /* Contabiliza o número de pacotes enviado por segundo. Ainda, ao passar de cada segundo, zera este valor.*/
    packages++;
    time(&final_time);
    if (difftime(final_time, initial_time) == 1.0){
      printf("%d %d\n", packages, t);
      time(&initial_time);
      t++;
      packages=0;
    }
  }

  // Fecha socket e termina programa
  printf("Fim da conexão\n");
  close(s);
  return 0;
}
