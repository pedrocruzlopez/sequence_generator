cmake_minimum_required (VERSION 2.6)

set (mysql_cmd "mysql_config")
set (mysql_arg "--cflags")
message(STATUS "mysql cmd: ${mysql_cmd}")
execute_process(COMMAND ${mysql_cmd} ${mysql_arg}
  RESULT_VARIABLE mysql_result
  OUTPUT_VARIABLE mysql_cflags_result)

string(REGEX REPLACE "\n$" ""  mysql_cflags_result "${mysql_cflags_result}")

if ("${mysql_cflags_result}" STREQUAL "")
	message(STATUS "seems like you don't have the program installed")
	set(mysql_flags "")
else()
	set (mysql_arg "--libs")
	
	execute_process(COMMAND ${mysql_cmd} ${mysql_arg}
  		RESULT_VARIABLE mysql_result
  		OUTPUT_VARIABLE mysql_libs)
	
	set(mysql_flags "${mysql_cflags_result}-D __MYSQL_H__")
endif()
string(REGEX REPLACE "\n$" ""  mysql_libs "${mysql_libs}")
message(STATUS "MySQL flags: ${mysql_flags}")
message(STATUS "MySQL libs: ${mysql_libs}")

set (psql_cmd "pg_config")
set (psql_arg "--includedir")

message(STATUS "psql cmd: ${psql_cmd}")
execute_process(COMMAND ${psql_cmd} ${psql_arg}
		RESULT_VARIABLE psql_result
		OUTPUT_VARIABLE psql_cflags_result)

string(REGEX REPLACE "\n$" ""  psql_cflags_result "${psql_cflags_result}")

if ("${psql_cflags_result}" STREQUAL "")
    message(STATUS "seems like you don't have the program installed")
    set(psql_flags "")
else()

    set(psql_flags "-I ${psql_cflags_result} -D __POSTGRESQL_H__")
endif()
message(STATUS "PostgreSQL flags: ${psql_flags}")


set (sequence_generator_src seqgen.c)
#add_definitions(${mysql_flags})
# Make build flags compiler specific
if (CMAKE_COMPILER_IS_GNUCC)
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		set (CMAKE_C_FLAGS "-D_REETRANT -ggdb -fPIC -Wall -Wextra -pedantic -O0 ${mysql_flags} ${psql_flags} -D DEV -D __CLION_DEBUG__")

		
		
		#set (CMAKE_EXE_LINKER_FLAGS  "${mysql_libs}" )
	elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
		set (CMAKE_C_FLAGS "-D_REETRANT -DNDEBUG -fPIC -Wall -Wextra -pedantic -O3 ${mysql_flags} ${psql_flags}")
		#set (CMAKE_EXE_LINKER_FLAGS, "${CMAKE_EXE_LINKER_FLAGS} -I/usr/include/mysql -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl ")
		
		#set (CMAKE_EXE_LINKER_FLAGS  "${mysql_libs}" )
	endif ()
endif (CMAKE_COMPILER_IS_GNUCC)


add_executable (seqgen ${sequence_generator_src})
if("${mysql_cflags_result}" STREQUAL "")
else()
	target_link_libraries(seqgen "${mysql_libs}")
endif()

if("${psql_cflags_result}" STREQUAL "")
else()
    target_link_libraries(seqgen "-lpq")
endif()

target_link_libraries(seqgen "-luuid")


#set (CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${mysql_libs}" )

#gcc -o seqgen seqgen.c $(mysql_config --cflags) $(mysql_config --libs) -D __MYSQL_H__ 
#						-I/usr/include/mysql     -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -lrt -ldl

# The rule to install sequence_generator binary
install (TARGETS seqgen
	RUNTIME
	DESTINATION bin)
