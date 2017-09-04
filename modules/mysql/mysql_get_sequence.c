 #ifdef STANDARD
 /* STANDARD is defined, don't use any mysql functions */
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 #ifdef __WIN__
 typedef unsigned __int64 ulonglong;/* Microsofts 64 bit types */
 typedef __int64 longlong;
 #else
 typedef unsigned long long ulonglong;
 typedef long long longlong;
 #endif /*__WIN__*/
 #else
 #include <my_global.h>
 #include <my_sys.h>
 #if defined(MYSQL_SERVER)
 #include <m_string.h>/* To get strmov() */
 #else
 /* when compiled as standalone */
 #include <string.h>
 #define strmov(a,b) stpcpy(a,b)
 #define bzero(a,b) memset(a,0,b)
 #define memcpy_fixed(a,b,c) memcpy(a,b,c)
 #endif
 #endif
 #include <mysql.h>
 #include <ctype.h>
 #include "mysql_sequence_header.h"
 #include <fcntl.h>		/* open */
 #include <unistd.h>		/* exit */
 #include <sys/ioctl.h>		/* ioctl */

   
#ifdef HAVE_DLOPEN
   
#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif
  


int file_desc = -1;


   
my_bool get_sequence_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
void get_sequence_deinit(UDF_INIT *initid __attribute__((unused)));
unsigned int get_sequence(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
unsigned int ioctl_get_msg(int file_desc, long long sequence);
   
my_bool get_sequence_init(UDF_INIT *initid, UDF_ARGS *args, char *message)  {
	if(!(args->arg_count == 1)) {
		strcpy(message, "Se espera un argumento (numero)");
		return 1;
	}
   
     if(file_desc == -1){
	 file_desc = open(DEVICE_FILE_PATH, 0);
     }
     return 0;
}
   

void get_sequence_deinit(UDF_INIT *initid __attribute__((unused)))  {
   
}


unsigned int ioctl_get_msg(int file_desc, long long sequence)
{
	
	auto unsigned int seq;

	ioctl(file_desc, sequence, &seq);

	return seq;
}
   
unsigned int get_sequence(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error){
	
  

	return ioctl_get_msg(file_desc, *((long long*) args->args[0]));
	
}


   
 #endif /* HAVE_DLOPEN */
   
