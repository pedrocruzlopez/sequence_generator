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
   
   my_bool calc_distance_udf_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
   void calc_distance_udf_deinit(UDF_INIT *initid __attribute__((unused)));
   int calc_distance_udf(UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error);
   
   my_bool calc_distance_udf_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
   {
     if(!(args->arg_count == 1)) {
       strcpy(message, "Se espera un argumento (comando)");
       return 1;
     }
   
     /*args->arg_type[0] = REAL_RESULT;*/
     /*args->arg_type[1] = REAL_RESULT;
     args->arg_type[2] = REAL_RESULT;
     args->arg_type[3] = REAL_RESULT;*/
   
     return 0;
   }
   
   void calc_distance_udf_deinit(UDF_INIT *initid __attribute__((unused)))
   {
   
   }


int ioctl_set_msg(int file_desc, int message)
{
	int ret_val;

	ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

	if (ret_val < 0) {
		printf("ioctl_set_msg failed:%d\n", ret_val);
		exit(-1);
	}
	return ret_val;
}

int ioctl_get_msg(int file_desc)
{
	int ret_val;
	int seq;
	

	/* 
	 * Warning - this is dangerous because we don't tell
	 * the kernel how far it's allowed to write, so it
	 * might overflow the buffer. In a real production
	 * program, we would have used two ioctls - one to tell
	 * the kernel the buffer length and another to give
	 * it the buffer to fill
	 */
	ret_val = ioctl(file_desc, IOCTL_GET_MSG, &seq);

	if (ret_val < 0) {
		return -1;
	}

	
	return seq;
}
   
  int calc_distance_udf(
	UDF_INIT *initid
,	UDF_ARGS *args
,	char* result
,	unsigned long* length
,	char *is_null
,	char *error
){
	
	
  
  	int ret_val;
	int seq;
	int file_desc;
	

	file_desc = open("/dev/seq_dev", 0);
	if (file_desc < 0) {
		return -1;
	}

	/*********************GET & SET VALUE****************************/
	seq = ioctl_get_msg(file_desc);
	
	
	/**********************************************************/

	
	
	close(file_desc);

	/*FILE * pFile;
	int number;
	pFile = fopen ("/proc/archivo_procesos","r");
	if (pFile!=NULL) {
		fscanf(pFile, "%d", &number);
		fclose (pFile);
	}
	*/
	

	return seq;
	
}
   
   #endif /* HAVE_DLOPEN */
   
