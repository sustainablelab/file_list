CFLAGS  = -Wall -Wextra -pedantic -std=c99
CFLAGS += `pkg-config --cflags sdl2`
LDLIBS  = `pkg-config --libs   sdl2`

# Pick which file to use as application.
# The recipes below generate info for that file.
MAIN = file_list

# View MAIN with macros expanded.
.PHONY: expand
expand: $(MAIN).c
	$(CC) $(CFLAGS) -E $^ > $(MAIN).txt

# Make a tags file for MAIN.
# This tags the source and the local header-only libs.
# Example: FileList is tagged, but SDL_Init is not.
# These tags include local variables.
.PHONY: tags
tags: $(MAIN).c parse-headers.exe
	$(CC) $(CFLAGS) $< -M > headers-M.txt
	./parse-headers.exe M
	echo $< >> headers.txt
	ctags --c-kinds=+l -L headers.txt

# Make a lib-tags file for MAIN.
# This tags all of the dependencies in MAIN, but not MAIN itself.
# This includes function prototypes.
# This does not include locals.
.PHONY: lib-tags
lib-tags: $(MAIN).c parse-headers.exe
	$(CC) $(CFLAGS) $< -M > headers-M.txt
	./parse-headers.exe
	ctags -f lib-tags --c-kinds=+p -L headers.txt

# Print human-readable tags.
# Include locals for MAIN.
.PHONY: show-tags
show-tags: $(MAIN).c parse-headers.exe
	@echo ---libs---
	@$(CC) $(CFLAGS) $< -M > headers.txt
	@./parse-headers.exe M
	@ctags -L headers.txt --sort=no -x
	@echo ---$(MAIN)---
	@ctags --c-kinds=+l   --sort=no -x $(MAIN).c

parse-headers.exe: parse-headers.c
	$(CC) -Wall $^ -o $@

.PHONY: clean
clean:
	rm -f $(MAIN).exe
	rm -f $(MAIN).txt
	rm -f parse-headers.exe
