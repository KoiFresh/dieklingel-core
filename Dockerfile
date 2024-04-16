FROM ubuntu:22.04

WORKDIR /app

COPY . .

RUN apt-get update && \
	apt-get install -y make && \
	make deps:all

RUN make clean package

CMD [ "exit" ]
