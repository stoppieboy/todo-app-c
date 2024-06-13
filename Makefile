build:
	@gcc todo.c -lglfw -lGL -lleif -lclipboard -lm -lxcb -o todo
run: build
	@./todo