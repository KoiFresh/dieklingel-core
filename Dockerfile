FROM ubuntu:22.04 as builder

WORKDIR /app

COPY . .

RUN apt-get update && \
	apt-get install -y make && \
	make deps:all

RUN make clean package

FROM ubuntu:22.04 as runner

COPY --from=builder /app/build/dieklingel-core*.deb /app/

RUN apt-get update && \
	apt-get install -y /app/dieklingel-core*.deb

ENTRYPOINT ["dieklingel-core"]
