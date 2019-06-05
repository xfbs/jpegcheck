jpegcheck: jpeg.o main.o qdbmp/qdbmp.o
	$(CC) -o jpegcheck $^

clean:
	$(RM) *.o jpegcheck
