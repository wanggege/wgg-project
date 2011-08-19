src = main.c init_op.c basic_shapes.c print_board.c mouse_op.c chess_op.c
para = -o main -Wall
main:$(src)
	gcc $(src) $(para)

clean:
	rm -rf main
	rm -rf *.o
	rm -rf ~*

 
