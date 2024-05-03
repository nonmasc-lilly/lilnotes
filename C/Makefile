all: src/*.c
	gcc src/*.c -o build/main -ansi
install: src/*.c ./LICENSE.md
	sudo gcc src/*.c -o /usr/bin/lilnotes -ansi
	@if [ ! -d "$(HOME)/.lilnotes" ]; then mkdir $(HOME)/.lilnotes; fi
	cp ./LICENSE.md $(HOME)/.lilnotes/LICENSE.md
test: all
	build/main
