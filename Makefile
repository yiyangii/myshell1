#Yi Yang, tuh34906 - Operating System Project 2
# CompLab2 tutor: Charles Chang
myshell: helper.c  implementedcommands.c  myshell.c  systemcommands.c helper.h  implementedcommands.h  myshell.h  systemcommands.h 
	gcc -g3 -Werror -Wall -Wpedantic helper.c  implementedcommands.c  myshell.c  systemcommands.c -o myshell

clean:
	rm -rf *.o && rm -rf myshell
