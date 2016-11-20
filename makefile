bmain = ./shareposd

row :
	make -s rownum

rownum :
	echo 'python代码行数 \c'
	find ${bmain} -name '*.py' | xargs grep -v "'^$'" | wc -l 
	echo 'c代码行数 \c'
	find ${bmain} -name "*.c" -or -name "*.h" | xargs grep -v "'^$'" | wc -l
	echo 'c++代码行数 \c'
	find ${bmain} -name "*.cpp" | xargs grep -v "'^$'" | wc -l
	echo 'total代码行数 \c'
	find ${bmain} -name '*.py' -or -name "*.c" -or -name "*.h" -or -name "*.cpp" | xargs grep -v "'^$'" | wc -l 




