main:
	gcc -o main main.c -static

main-dont-use-yet: main.c
	gcc main.c -o main.o -c
	gcc src/pager.c -o pager.o -c
	gcc -o main main.o pager.o

run: main
	./main

clean:
	rm -f main combined_main.c *.o *.db

format: *.c
	clang-format -style=Google -i *.c


test:
	python3 tests/main.py

kill:
	sudo kill -9 $(sudo lsof -t -i:8203)


simple:
	docker-compose up --build -d simple

nginx:
	docker-compose up --build -d nginx

python:
	docker-compose up --build -d python

up:
	docker-compose up --build -d

down:
	docker-compose down

simple-cli:
	docker exec -it simple_service /bin/sh
