EXECUTABLE = c-wildwater

src: $(EXECUTABLE)

$(EXECUTABLE): code/histo.c
	@echo "Compilation du programme"
	gcc -o $(EXECUTABLE) code/histo.c
	@echo "Compilation réussi: Executable ./"$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) a.out
