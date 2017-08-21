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
   #include "sequence_driver.h"
   #include <fcntl.h>		/* open */
   #include <unistd.h>		/* exit */
   #include <linux/ioctl.h>		/* ioctl */

   
   #ifdef HAVE_DLOPEN
   
   #if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
   static pthread_mutex_t LOCK_hostname;
   #endif
   
   #include <math.h>
   
   my_bool get_sequence_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
   void calc_distance_deinit(UDF_INIT *initid __attribute__((unused)));
   int get_sequence(UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error);
   
   my_bool get_sequence_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
   {
     if(!(args->arg_count == 1)) {
       strcpy(message, "Se espera un argumento (numero)");
       return 1;
     }
   
     return 0;
   }
   
   void get_sequence_deinit(UDF_INIT *initid __attribute__((unused)))
   {
   
   }



int ioctl_get_msg(int file_desc, long long sequence)
{
	int ret_val;
	int seq;
	
	ret_val = ioctl(file_desc, sequence, &seq);

	return seq;
}
   
  int get_sequence(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char *is_null
,	char *error
){
	
	
  
  	int ret_val;
	int seq;
	int file_desc;
	

	file_desc = open("/dev/seq_dev", 0);
	

	
	seq = ioctl_get_msg(file_desc, *((long long*) args->args[0]));
	
	
		
	
	close(file_desc);

	
	

	return seq;
	
}
   
   #endif /* HAVE_DLOPEN */
   
