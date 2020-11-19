#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char*argv[])
{
    int s_ecoute,scom,lg_app,i,j;
    struct sockaddr_in adr;
    struct sockaddr_storage recep;
    char buf[1500], renvoi[1500],host[1024],service[20];

    s_ecoute = socket(AF_INET,SOCK_STREAM,0);

    printf("File Descriptor = %d\n",s_ecoute);

    adr.sin_family=AF_INET;
    adr.sin_port=htons((atoi(argv[1])));
    adr.sin_addr.s_addr=INADDR_ANY;

    printf("adr.sin_port = %d\n",adr.sin_port);

    if (bind(s_ecoute,(struct sockaddr *)&adr,sizeof(struct sockaddr_in)) != 0) {
        printf("Problème lors du bind sur v4\n");
        exit(1);
    }

    if(listen(s_ecoute,5)!=0){
        printf("Problème lors de l'ecoute\n");
        exit(1);
    }

    printf("### AWAITING CONNECTION ###\n\n");

    while(1) {
        scom = accept(s_ecoute,(struct sockaddr*)&recep,(unsigned long *)&lg_app);
        getnameinfo((struct sockaddr *)&recep,sizeof(recep),host,sizeof(host),service,sizeof(service),0);
        printf("Recu de %s venant du port %s\n",host,service);
        
        while(1)
        {
            recv(scom,buf,sizeof(buf),0);
            printf("buf recu %s\n",buf);
            bzero(renvoi,sizeof(renvoi));
            for(i = strlen(buf)-1,j=0;i>=0;i--,j++) renvoi[j]=buf[i];
            renvoi[j+1]='\0';

            send(scom,renvoi,strlen(renvoi),0);
            bzero(buf,sizeof(buf));
            if(strcmp(renvoi,"NIF")==0) break;
        }
        close(scom);
    }
    close(s_ecoute);
    
}
