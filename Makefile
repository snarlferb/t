CC=gcc
message_system: message_system.c
	@touch message_system.c  # Update the modification timestamp after cleaned
	$(CC) -g -o message_system message_system.c
clean:
	rm -f message_system
.PHONY: build_msg
build_msg:
	@printf "#\n# Building \n#\n"
