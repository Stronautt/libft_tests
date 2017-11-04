#!/bin/sh

if [[ $1 = "bonus" ]]; then
	echo "!Start TESTING BONUS PART!\n";

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
		echo "\n!TEST!\t--\tCOMPILING ERROR\t--\t[ABORT]\n"
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
	echo "!!!!CAUTION\t--\tSELF DESTRUCTION!!!!"
	rm -f test.c test_b.c check.sh;	
elif [[ -z $1  ]] || [[ $1 = "debug" ]]; then
	#Deleting old testing binary
	rm -f test.out;
	
	if [[ $1 != "debug" ]]; then
		if [[ -n "$(find ../ -name "author")" ]]; then
			AUTHOR_FILE="author"
		elif [[ -n "$(find ../ -name "auteur")" ]]; then
			AUTHOR_FILE="auteur"
		else
			echo "!NO auteur or author file found!\n";
		fi

		if [[ -n $AUTHOR_FILE ]]; then
			if [[ $(cat ../$AUTHOR_FILE) = $(whoami) ]]; then
				echo "AUTHOR IS\t--\t"$(cat ../$AUTHOR_FILE)"\t--\t[OK]\n";
			else
				echo "AUTHOR IS\t--\t"$(cat ../$AUTHOR_FILE)"\t--\t[STOLEN]\n";
			fi
		fi		

		#Checking for Norm errors all source files
		NORM_ERRORS=$(norminette ../*.c ../*.h | grep "^Error\|^Warning" -B 1);

		if [[ -n "${NORM_ERRORS}" ]]; then
			echo "!TEST!\t--\tNORMINETTE\t--\t[FAILED]\n";
			echo ${NORM_ERRORS}"
		\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		||||||||||||||||||||||||||||||||||||||||||||||\n";
		else
			echo "!TEST!\t--\tNORMINETTE\t--\t[OK]\n";
		fi
		
		#Deleting old library
		rm -f libft.a;

		#Deleting old objective files
		rm -f *.o

		#Checking up for Makefile
		if [[ -z "$(find ../ -name "Makefile")" ]]; then
			echo "!NO Makefile file found!\n";
		else
			CURRDIR=$(pwd)
			cd ../;
			MAKEFILE=$(make > /dev/null && make);
			make clean > /dev/null;
			if [[ $MAKEFILE != "make: Nothing to be done for \`all'." ]]; then
				echo "!TEST!\t--\tMAKEFILE\t--\t[FAILED]\n";
			elif [[ -z "$(find . -name "libft.a")" ]]; then
				echo "!TEST!\t--\tMAKEFILE\t--\t[FAILED]\n";
			else
				echo "!TEST!\t--\tMAKEFILE\t--\t[OK]\n";
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
			echo "\n!TEST!\t--\tCOMPILING ERROR\t--\t[ABORT]\n"
			exit $COMPILING;
		fi
	fi

	if [[ -z "$(find . -name "test.out")" ]]; then
		if [[ -z "$(find . -name "test.c")" ]]
		then
			echo "!TEST!\t--\tTEST PROGRAM MISSING\t--\t[ABORT]\n";
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