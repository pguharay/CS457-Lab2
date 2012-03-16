all :
	mkdir ./obj;make -f makess; make -f makeawget;
awget:
	mkdir ./obj; make -f makeawget;
ss:
	mkdir ./obj; make -f makess;	
clean:
	rm -rf 	./obj;rm -f ss;rm -f awget;

	
