#!/bin/bash
#A shell script with in- and output
#Lars
#28.10.2019
VERSION="1.0"

usage() {
	cat <<EOF
	$thiscommand [ OPTIONS ] FILE [ FILE . . . ]
	Sort filenames by file type as given by
	the ''file'' command
	$thiscommand --version	Print Version number
	OPTIONS :
	-t		sort text files only
	-n		sort non-text files only
	-r		reverse sort order
	-v --verbose	print debugging messages   
    
EOF
}

sortText() {
	if [ "$reverse" == "true" ] ; 
		then file $* | grep ':.*text' | sort -rk2
	 else
	 	file $* | grep ':.*text' | sort -k 2
	fi
}


sortFiles() {
	if [ "$reverse" == "true" ] ;
                then file $* | grep -v ':.*text'| sort -rk2
         else
                file $* | grep -v ':.*text' | sort -k2
        fi
	
}

sortAll() {
	if [ "$reverse" == "true" ] ;
                then file $* | sort -rk2
         else
                file $* | sort -k2
        fi
	
}

# ###########################################################
#			main
# check for options
## note the blanks after ‘[’ and before ‘]’

text=false
nonText=false
reverse=false
verbose=false
optioncounter= 0

while [[ $1 == -* ]]
do
	
	optioncounter=$((optionscounter++))
	case $1 in
		"-h" | "--help")
			usage
			exit 0
			;;

		"--version")
			echo "Version: $VERSION"
			exit 0
			;;

		"-t")
			text=true
			;;

		"-n")
			nonText=true
			;;

		"-r")
			reverse=true
			;;

		"-v")
			verbose=true
			
			;;


		*)
		    usage
		    exit 
			;;
	esac
	shift
done

if [ "$verbose" == "true" ] ;
	then echo "text = $text, nonText = $nonText, reverse = $reverse, verbose = $verbose"
fi

#---fehlersuche
#-n und -t gleichzeitig abfrage
if [ "$text" == "true" ]  && [ "$nonText" == "true" ]  ;
	then exit 0	
fi

#keine argumente abfrage
let "optionscounter-=$#"
if [ $optioncounter -le 0 ] ;
	then exit 0
fi
#----fertig mit Fehlerabfraegn


if [ "$text" == "true" ] ;
	then sortText $*

elif [ "$nonText" == "true" ] ;
	then sortFiles $*
else 
	sortAll $*

fi



exit 0
