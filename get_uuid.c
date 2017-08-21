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
 #include <fcntl.h>		/* open */
 #include <unistd.h>		/* exit */


   
#ifdef HAVE_DLOPEN
   
#if !defined(HAVE_GETHOSTBYADDR_R) || !defined(HAVE_SOLARIS_STYLE_GETHOST)
static pthread_mutex_t LOCK_hostname;
#endif
  
#include <math.h>
   

my_bool get_uuid_init (UDF_INIT *initid, UDF_ARGS *args, char *message);
void get_uuid_deinit(UDF_INIT *initid __attribute__((unused)));
char *get_uuid (UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);

my_bool get_uuid_init(UDF_INIT *initid, UDF_ARGS *args, char *message){
	return 0;
}

void get_uuid_deinit(UDF_INIT *initid __attribute__((unused))){

}




char *get_uuid (UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error){

	return "hola";
}
   
 #endif /* HAVE_DLOPEN */
   
