build : /home/laojing/Projects/SmartHome/Source/MainFrame/Main.c
	gcc -o .exec -std=gnu99 `pkg-config --cflags --libs glib-2.0`/home/laojing/Projects/SmartHome/Source/MainFrame/Main.c
run : build
	./.exec 