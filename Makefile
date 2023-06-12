.PHONY:		all clean fclean re test configure
.SUFFIXES:	.s .o

#################################################
#					PROJECT						#
#################################################

NAME := 	libasm.a
TESTER :=	libasm_test

OBJS :=		src/ft_strlen.o		\
			src/ft_strcpy.o		\
			src/ft_strcmp.o		\
			src/ft_read.o		\
			src/ft_write.o		\
			src/ft_strdup.o

#################################################
#				TOOLS CONFIGURATION				#
#################################################

AS :=		nasm
AR :=		ar

ASFLAGS :=	-g
ARFLAGS :=	-rcs

#################################################
#			OS DEPENDENT CONFIGURATION			#
#################################################

OS :=		$(shell uname)

ifeq ($(OS), Darwin)
CONF :=		conf/darwin_conf.s
ASFLAGS +=	-fmacho64
else
CONF :=		conf/linux_conf.s
ASFLAGS +=	-felf64
endif

all:		$(NAME)

$(NAME):	$(OBJS)
	@echo -e "\033[34mLinking $@...\033[0m"
	@$(AR) $(ARFLAGS) $@ $(OBJS)

conf.s: $(CONF)
	@echo -e "\033[31mConfiguring...\033[0m"
	@cp $(CONF) conf.s

%.o: %.s 	conf.s
	@echo -e "\033[31mCompiling $<...\033[0m"
	@$(AS) $(ASFLAGS) -o $@ $<

re: fclean all

clean:
	@echo -e "\033[31mCleaning compilation files...\033[0m"
	@rm -f $(OBJS)
	@rm -f conf.s

fclean:
	@echo -e "\033[31mRemoving binaries...\033[0m"
	@rm -f $(NAME) $(TESTER)

test: all
	@echo -e "\033[31mCompiling tester...\033[0m"
	@gcc -ggdb -o $(TESTER) main.c libasm.a
	@echo -e "\033[34mStarting tester...\033[0m"
	@./$(TESTER)
	@rm $(TESTER)
