/* ********************************************** */
/* Na podstawie: http://xmlrpc-c.sourceforge.net  */
/* ********************************************** */
#define WIN32_LEAN_AND_MEAN  /* wymagane przez xmlrpc-c/server_abyss.h */

#include <stdlib.h>
#include <stdio.h>
#include <xmlrpc-c/base.h>
#include <xmlrpc-c/server.h>
#include <xmlrpc-c/server_abyss.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//======================================================================
int error( int ern, const char *err ){
	perror( err );
	return ern;
}
int snd(char * snd)
{
	int 		n;		
	int 		fd;
	socklen_t 	socklen;
	
	struct 	sockaddr_in serv_addr, cli_addr;
	
	/* tworzymy gniazdo ***********************************************/
	fd = socket( AF_INET, SOCK_STREAM, 0 );		// tworzymy nowe gniazdo
	if (fd < 0) 
        return error( 1, "socket()" );
        
    /* zapelniamy strukture zerami (inicjujemy) ***********************/
	bzero( ( char * ) &serv_addr, sizeof( serv_addr ) );	// zapelniamy strukture zerami
	
	/* przypisujemy parametry *****************************************/
	serv_addr.sin_family 	  	= AF_INET;			// typ gniazda 
	serv_addr.sin_addr.s_addr 	= INADDR_ANY;		// oczekujemy polaczen na kazdym adresie
	serv_addr.sin_port 			= htons(10004);		// port, na ktorym nasluchujemy
	
	/* kod obslugujacy nowe polaczenia ********************************/
	socklen = sizeof(cli_addr);
	/* nawiazujemy polaczenie *****************************************/
    if( connect( fd, (struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) 
        return error( 3, "connect()" );

	n = write( fd, snd, 1024 );
	if( n < 0 )
		perror( "write()" );

	close(fd);
    return 0;
}
//======================================================================
static xmlrpc_value * recvv(
			xmlrpc_env *   const envP,
			xmlrpc_value * const paramArrayP,
			void *         const serverInfo,
			void *         const channelInfo) {

    xmlrpc_value * x;

    /* Parse our argument array. */
    xmlrpc_decompose_value(envP, paramArrayP, "(s)", &x);
    if (envP->fault_occurred)
        return NULL;


    // TODO debug
    // printf(x);
    // printf("\n");
    // fflush(stdout);

    snd((char *)x);

    /* Return our result. */
    return xmlrpc_build_value(envP, "i", 0);
}
//======================================================================
int main( void ){

	/* zmienne predefiniowane *****************************************/
	int  port   = 10003;
	//------------------------------------------------------------------
    struct xmlrpc_method_info3 const methodInfo = {
        /* .methodName     = */ "recvv",
        /* .methodFunction = */ &recvv,
    };
    
	/* zmienne niezainicjowane ****************************************/
    xmlrpc_server_abyss_parms serverparm;
    xmlrpc_registry * registryP;
    xmlrpc_env env;

    /* inicjujemy serwer **********************************************/
    xmlrpc_env_init(&env);

	/* rejestrujemy zmienne srodowiskowe ******************************/
    registryP = xmlrpc_registry_new(&env);
    if (env.fault_occurred){
        printf( "xmlrpc_registry_new(): %s\n", env.fault_string);
        exit(1);
    }

	/* rejestrujemy nasza metode/funkcje ******************************/
    xmlrpc_registry_add_method3( &env, registryP, &methodInfo );
    if (env.fault_occurred) {
        printf( "xmlrpc_registry_add_method3(): %s\n", env.fault_string );
        exit(1);
    }

	/* ustawiamy parametry serwera ************************************/
    serverparm.config_file_name = NULL;
    serverparm.registryP        = registryP;
    serverparm.port_number      = port;
    serverparm.log_file_name    = "/tmp/xmlrpc_log";

	/* uruchamiamy serwer *********************************************/
    xmlrpc_server_abyss(&env, &serverparm, XMLRPC_APSIZE(log_file_name));
    if( env.fault_occurred ){
        printf("xmlrpc_server_abyss(): %s\n", env.fault_string);
        exit(1);
    }

	/* konczymy dzialanie *********************************************/
    return 0;
}
//======================================================================


