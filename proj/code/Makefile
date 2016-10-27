# Makefile for the project

COMPILER_TYPE= gnu

CC=gcc

PROG=	proj
SRCS=	proj.c vbe.c video_gr.c timer.c keyboard.c maze.c bitmap.c Daedalus.c sprite.c rtc.c date.c mouse.c

CCFLAGS= -Wall 

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>
