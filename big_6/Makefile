HELPER_EXE =	GF_lookup

HELPER_SRCS =
HELPER_SRCS	+=	make_GF_array.c
HELPER_SRCS	+=	GF.c
HELPER_SRCS	+=	GF.h

HELPER_OBJS	= $(patsubst %.c,%.o,$(filter %.c,${HELPER_SRCS}))

HELPER_DEPS	=	$(patsubst %.o,%.d,${HELPER_OBJS})

EXE	=	executable

SRCS	=
SRCS	+=	main.c
SRCS	+=	recovery.c
SRCS	+=	recovery.h
SRCS	+=	GF_lookup_tables.c
SRCS	+=	GF_lookup_tables.h

LIBS	=
#LIBS	+=	-lm


OBJS	=	$(patsubst %.c,%.o,$(filter %.c,${SRCS}))

DEPS	=	$(patsubst %.o,%.d,${OBJS})

CC	=	gcc

CFLAGS	=
CFLAGS	+=	-MMD
CFLAGS	+=	-O3
CFLAGS	+=	-Wall
CFLAGS	+=	-Werror
CFLAGS	+=	-Wextra
#CFLAGS	+=	-std=c2x
CFLAGS	+=	-std=gnu2x

LD	=	gcc

LDFLAGS	=

all:	${EXE} 

.PHONY:	all

clean:
	rm -f ${OBJS}
	rm -f ${DEPS}
	rm -f ${HELPER_OBJS}
	rm -f ${HELPER_DEPS}

.PHONY:	clean 

distclean:	clean
	rm -f ${EXE}
	rm -f ${HELPER_EXE}

.PHONY:	distclean

${EXE}: ${OBJS}
	${LD} ${LDFLAGS} $^ ${LIBS} -o $@

%.o:	%.c
	${CC} ${CFLAGS} -c $< -o $@

GF_lookup_tables.c GF_lookup_tables.h : ${HELPER_EXE}
	./${HELPER_EXE}

main.o: main.c GF_lookup_tables.h recovery.h

${HELPER_EXE}: ${HELPER_OBJS}
	${LD} ${LDFLAGS} $^ ${LIBS} -o $@

-include ${DEPS}
