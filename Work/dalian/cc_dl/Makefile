#DFLAG=-DDEBUG
#DFLAG=
OBJECTS = base_md5.o base_crc.o  base_message.o base_socket.o base_funcs.o base_cjson.o base_xml.o  cc_main_dl.o cc_ora_dl.o cc_commu_dl.o cc_base_dl.o
LIBES = -lpthread -lrt
GCC= g++ -c

PROC=proc

PROC_INCLUDE=include=./ include=/usr/include/ include=/usr/include/sys/ include=/usr/lib/gcc/x86_64-redhat-linux/4.4.4/include/

DB_LIB= -L$(ORACLE_HOME)/lib/ -lclntsh `cat $(ORACLE_HOME)/lib/ldflags`   `cat $(ORACLE_HOME)/lib/sysliblist` -lm

DB_LFLAG = -DSLTS_ENABLE -D_REENTRANT   -I. -I$(ORACLE_HOME)/precomp/public -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public 

RM = rm -f

#Create target file
all:metro_center clean

metro_center : $(OBJECTS)
	g++ $(OBJECTS) -ldl -lm $(LIBES) $(DB_LFLAG) $(DB_LIB) -o $@
	 
base_md5.o :      base_md5.cpp base_md5.h sys_head.h
	$(GCC) $< -o $@

base_crc.o :      base_crc.cpp base_crc.h sys_head.h
	$(GCC) $< -o $@
		
base_message.o :  base_message.cpp base_message.h sys_head.h
	$(GCC) $< -o $@
	
base_funcs.o :  	base_funcs.cpp base_funcs.h sys_head.h base_define.h
	$(GCC) $< -o $@
	
base_socket.o :  	base_socket.cpp base_socket.h sys_head.h
	$(GCC) $< -o $@

base_cjson.o :  	base_cjson.cpp base_socket.h sys_head.h
	$(GCC) $< -o $@
	
base_xml.o :  	base_xml.cpp base_socket.h sys_head.h
	$(GCC) $< -o $@

cc_base_dl.o :  	cc_base_dl.cpp base_funcs.h sys_head.h
	$(GCC) $< -o $@	
	
cc_commu_dl.o : cc_commu_dl.cpp sys_head.h  base_define.h base_message.h base_md5.h base_socket.h cc_commu_dl.h
	$(GCC) $< -o $@

cc_main_dl.o :  cc_main_dl.cpp sys_head.h base_funcs.h base_md5.h base_crc.h base_socket.h base_cjson.h base_xml.h cc_struct_dl.h cc_ora_dl.h cc_commu_dl.h cc_base_dl.h
	$(GCC) $< -o $@	

cc_ora_dl.o:    cc_ora_dl.pc  sys_head.h base_funcs.h base_socket.h base_cjson.h base_xml.h cc_struct_dl.h cc_ora_dl.h cc_commu_dl.h cc_base_dl.h
	$(PROC) code=cpp sqlcheck=semantics user=dlmlcdb/dlmlcdb iname=cc_ora_dl.pc oname=cc_ora_dl.cpp $(PROC_INCLUDE)
	$(GCC) cc_ora_dl.cpp $(DB_LFLAG) 
 
#clean temp file
.PHONY : clean
clean:
	g++ base_dog.cpp base_socket.cpp -o dog
	chmod u+s dog
	$(RM) *.o
	$(RM) *.lis
	$(RM) *.pc
	$(RM) *.h
	$(RM) *.cpp
	$(RM) tp*
	
	
	
