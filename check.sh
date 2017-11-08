#!/bin/sh

centerQ()
{
	textsize=${#1}
	width=$(tput cols)
	span=$((($width + $textsize) / 2))
	printf "%${span}s" "$1"
}

#Color Library
#START
NC="\033[0m"; GREEN="\033[32m"; RED="\033[31m"; FRED="\033[41m" YELLOW="\033[33m";
LCYAN="\033[96m"; BLUE="\033[34m"; BOLD="\033[1m"; FGREEN="\033[42m";
FBLUE="\033[44m"; LMAGENTA="\033[95m"; DEFAULT="\033[39m"; FWHITE="\033[107m"; BLACK="\033[30m"; WHITE="\033[97m"; BROWN="\033[38;5;166"
#END

if [[ $1 = "bonus" ]]; then
	echo "${FBLUE}\n${NC}${LCYAN}
     ____   ___  _   _ _   _ ____    ____   _    ____ _____ 
    | __ ) / _ \| \ | | | | / ___|  |  _ \ / \  |  _ |_   _|
    |  _ \| | | |  \| | | | \___ \  | |_) / _ \ | |_) || |  
    | |_) | |_| | |\  | |_| |___) | |  __/ ___ \|  _ < | |  
    |____/ \___/|_| \_|\___/|____/  |_| /_/   \_|_| \_\|_|${NC}\n${FBLUE}\n${NC}\n";

	#Deleting old library
	rm -f libft_b.a;

	#Deleting Makefile objective files
	rm -f *.o

	gcc -Wextra -Wall -Werror -c ../*.c;
	COMPILING=$?

	if [[ "${COMPILING}" -eq 0 ]]; then
		#Packing library
		ar rc libft_b.a *.o;
		rm -f *.o;
		ranlib libft_b.a;
	else
		rm -f *.o;
		echo "\n${YELLOW}!TEST!\t--\tCOMPILING ERROR\t--\t[ABORT]${NC}\n"
		exit $COMPILING;
	fi

	#Deleting old testing binary
	rm -f test_b.out;

	#Compiling test program
	gcc -w test_b.c libft_b.a -o test_b.out;

	#Set up rules for execution for test file
	chmod 777 test_b.out;

	#Run testing
	./test_b.out;

	#Clean trash
	rm -f libft_b.a test_b.out;
elif [[ $1 = "boom" ]]; then
	echo "${FRED}
╔═╗╔═╗╦  ╔═╗  ╔╦╗╔═╗╔═╗╔╦╗╦═╗╦ ╦╔═╗╔╦╗╦╔═╗╔╗╔
╚═╗║╣ ║  ╠╣    ║║║╣ ╚═╗ ║ ╠╦╝║ ║║   ║ ║║ ║║║║
╚═╝╚═╝╩═╝╚    ═╩╝╚═╝╚═╝ ╩ ╩╚═╚═╝╚═╝ ╩ ╩╚═╝╝╚╝${NC}";

	rm -f test.c test_b.c check.sh;	
elif [[ -z $1  ]] || [[ $1 = "debug" ]]; then
	echo "${FBLUE}\n${NC}${GREEN}


  ██╗     ██╗██████╗ ███████╗████████╗    ████████╗███████╗███████╗████████╗
  ██║     ██║██╔══██╗██╔════╝╚══██╔══╝    ╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝
  ██║     ██║██████╔╝█████╗     ██║          ██║   █████╗  ███████╗   ██║   
  ██║     ██║██╔══██╗██╔══╝     ██║          ██║   ██╔══╝  ╚════██║   ██║   
  ███████╗██║██████╔╝██║        ██║          ██║   ███████╗███████║   ██║   
  ╚══════╝╚═╝╚═════╝ ╚═╝        ╚═╝          ╚═╝   ╚══════╝╚══════╝   ╚═╝ 


  ${WHITE}v1.03 © pgritsen
  https://github.com/CoZZmOnAvT/libft_tests
${FBLUE}\n${NC}\n\n";

	#Deleting old testing binary
	rm -f test.out;
	
	if [[ $1 != "debug" ]]; then
		if [[ -n "$(find ../ -name "author")" ]]; then
			AUTHOR_FILE="author"
		elif [[ -n "$(find ../ -name "auteur")" ]]; then
			AUTHOR_FILE="auteur"
		else
			echo "${FRED}!NO auteur or author file found!${NC}\n";
		fi

		if [[ -n $AUTHOR_FILE ]]; then
			if [[ $(cat ../$AUTHOR_FILE) = $(whoami) ]]; then
				echo "${GREEN}AUTHOR IS\t--\t"$(cat ../$AUTHOR_FILE)"\t--\t[OK]${NC}\n";
			else
				echo "${RED}AUTHOR IS\t--\t"$(cat ../$AUTHOR_FILE)"\t--\t[STOLEN]${NC}\n${RED}"'
  ██████ ▄▄▄█████▓ ▒█████   ██▓    ▓█████  ███▄    █ 
▒██    ▒ ▓  ██▒ ▓▒▒██▒  ██▒▓██▒    ▓█   ▀  ██ ▀█   █ 
░ ▓██▄   ▒ ▓██░ ▒░▒██░  ██▒▒██░    ▒███   ▓██  ▀█ ██▒
  ▒   ██▒░ ▓██▓ ░ ▒██   ██░▒██░    ▒▓█  ▄ ▓██▒  ▐▌██▒
▒██████▒▒  ▒██▒ ░ ░ ████▓▒░░██████▒░▒████▒▒██░   ▓██░
▒ ▒▓▒ ▒ ░  ▒ ░░   ░ ▒░▒░▒░ ░ ▒░▓  ░░░ ▒░ ░░ ▒░   ▒ ▒ 
░ ░▒  ░ ░    ░      ░ ▒ ▒░ ░ ░ ▒  ░ ░ ░  ░░ ░░   ░ ▒░
░  ░  ░    ░      ░ ░ ░ ▒    ░ ░      ░      ░   ░ ░ 
      ░               ░ ░      ░  ░   ░  ░         ░ 
                                                     '"${NC}\n";
			fi
		fi		

		#Checking for Norm errors all source files
		NORM_ERRORS=$(norminette ../*.c ../*.h | grep "^Error\|^Warning" -B 1);

		if [[ -n "${NORM_ERRORS}" ]]; then
			echo "${RED}!TEST!\t--\tNORMINETTE\t--\t[FAILED]${NC}\n";
			echo ${FRED}${NORM_ERRORS}${NC}"\n"${RED}"
         ／＞　   フ
         | 　_　 _|
        ／\`ミ _x 彡
       /　　　 　 |
      /　 ヽ　　 ﾉ
  ／￣|　 |　|　|
  | (￣ヽ＿_ヽ_)_)
  ＼二つ ${NC}\n"
		else
			echo "${GREEN}!TEST!\t--\tNORMINETTE\t--\t[OK]${NC}\n";
		fi
		
		#Deleting old library
		rm -f libft.a;

		#Deleting old objective files
		rm -f *.o

		#Checking up for Makefile
		if [[ -z "$(find ../ -name "Makefile")" ]]; then
			echo "${RED}!NO Makefile file found!${NC}\n";
		else
			CURRDIR=$(pwd)
			cd ../;
			MAKEFILE=$(make > /dev/null && make);
			make clean > /dev/null;
			if [[ $MAKEFILE != "make: Nothing to be done for \`all'." ]]; then
				echo "${RED}!TEST!\t--\tMAKEFILE\t--\t[FAILED]${NC}\n
${RED}
         ／＞　   フ
         | 　_　 _|
        ／\`ミ _x 彡
       /　　　 　 |
      /　 ヽ　　 ﾉ
  ／￣|　 |　|　|
  | (￣ヽ＿_ヽ_)_)
  ＼二つ ${NC}";
			elif [[ -z "$(find . -name "libft.a")" ]]; then
				echo "${RED}!TEST!\t--\tMAKEFILE\t--\t[FAILED]${NC}\n
${RED}
         ／＞　   フ
         | 　_　 _|
        ／\`ミ _x 彡
       /　　　 　 |
      /　 ヽ　　 ﾉ
  ／￣|　 |　|　|
  | (￣ヽ＿_ヽ_)_)
  ＼二つ ${NC}";
			else
				echo "${GREEN}!TEST!\t--\tMAKEFILE\t--\t[OK]${NC}\n";
			fi
			make fclean > /dev/null;
			cd $CURRDIR;
		fi
	fi

	if [[ -z "$(find . -name "libft.a")" ]]; then
		#Creating objective files
		gcc -Wextra -Wall -Werror -c ../*.c;
		COMPILING=$?

		if [[ "${COMPILING}" -eq 0 ]]; then
			#Packing library
			ar rc libft.a *.o;
			rm -f *.o;
			ranlib libft.a;
		else
			rm -f *.o;
			echo "\n${RED}!TEST!\t--\tCOMPILING ERROR\t--\t[ABORT]${NC}\n${RED}
╔═╗╦ ╦╦╔╦╗┬
╚═╗╠═╣║ ║ │
╚═╝╩ ╩╩ ╩ o${NC}\n"
			exit $COMPILING;
		fi
	fi

	if [[ -z "$(find . -name "test.out")" ]]; then
		if [[ -z "$(find . -name "test.c")" ]]
		then
			echo "${YELLOW}!TEST!\t--\tTEST PROGRAM MISSING\t--\t[ABORT]${NC}\n
╔═╗╦ ╦╔═╗╦╔═ 
╠╣ ║ ║║  ╠╩╗ 
╚  ╚═╝╚═╝╩ ╩o\n";
		else
			#Compiling test source file with library
			gcc -w test.c libft.a -o test.out;
		fi
	fi

	#Set up rules for execution for test file
	chmod 777 test.out;

	#Run testing
	./test.out;	

	if [[ $1 != "debug" ]]; then
		#Clean trash
		rm -f libft.a test.out;	
	fi
fi