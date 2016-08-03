build : /home/laojing/arm11/boa/cgi/loop/Source/Gui/ComSocket.c
	gcc -o .exec -std=gnu99 `pkg-config --cflags --libs glib-2.0`/home/laojing/arm11/boa/cgi/loop/Source/Gui/ComSocket.c
run : build
	./.exec 