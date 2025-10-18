TARGET:=main
CC:=arm-linux-gcc
#保存工程中所有的.c文件
SRCS:= $(wildcard ./src/*.c)
#保存工程里面所有用到的.o文件
OBJS:= $(patsubst %.c,%.o,$(SRCS))
#保存工程中所有头文件的搜索路径
INCS:= -I ./inc
#保存工程中库的搜索路径
LIBS:= -L ./lib -lpthread -lm
$(TARGET):$(OBJS)
	$(CC) $^ $(LIBS) -o $@
%.o:%.c
	$(CC) -c $< $(INCS) -o $@
	
clean:
	rm -rf $(OBJS) 
	rm -rf $(TARGET)