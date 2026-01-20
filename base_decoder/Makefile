EXE	=	shepsalbasety

SRCS	=
SRCS	+=	main.c

SRCS	+= 	maths.c
SRCS 	+= 	maths.h

SRCS	+=	generic_encode.c 
SRCS 	+=	generic_encode.h

SRCS	+=	generic_decode.c
SRCS	+=	generic_decode.h

SRCS	+=	base2.h
SRCS	+=	base4.h 
SRCS	+=	base8.h
SRCS	+=	base16.h
SRCS	+=	base32.h
SRCS	+=  base64.h

#SRCS    += 	base32_dec.c
#SRCS    += 	base32_dec.h

#SRCS    += 	base32_enc.c
#SRCS    += 	base32_enc.h

#SRCS	+= 	base64_enc.c
#SRCS	+= 	base64_enc.h

#SRCS	+=  base64_dec.c
#SRCS	+=  base64_dec.h



LIBS	=
#LIBS	+=	-lm

OBJS	=	$(patsubst %.c,%.o,$(filter %.c,${SRCS}))

DEPS	=	$(patsubst %.o,%.d,${OBJS})

CC	=	gcc

CFLAGS	=
CFLAGS	+=	-MMD
CFLAGS	+=	-O3      #use when compiling finished product
CFLAGS	+=	-Wall
CFLAGS	+=	-Werror
CFLAGS	+=	-Wextra
CFLAGS	+=  -fsanitize=address
#CFLAGS 	+=  -O0  #use while debugging  
CFLAGS	+= 	-g       #debug info included 
#CFLAGS	+=	-std=c2x
CFLAGS	+=	-std=gnu2x

LD	=	gcc

LDFLAGS	=	-fsanitize=address


all:	${EXE}

.PHONY:	all

clean:
	rm -f ${OBJS}
	rm -f ${DEPS}

.PHONY:	clean 

distclean:	clean
	rm -f ${EXE}

.PHONY:	distclean

${EXE}: ${OBJS}
	${LD} ${LDFLAGS} $^ ${LIBS} -o $@

%.o:	%.c
	${CC} ${CFLAGS} -c $< -o $@

-include ${DEPS}
