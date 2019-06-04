jpegcheck: jpeg.o main.o
	$(CC) -o jpegcheck $^

clean:
	$(RM) *.o jpegcheck
