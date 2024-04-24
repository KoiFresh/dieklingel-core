FROM ubuntu:22.04 as builder

WORKDIR /app

COPY . .

RUN apt-get update && \
	bash taskfile init && \
	./taskfile deps:all

RUN ./taskfile clean package --with-libcamera

FROM ubuntu:22.04 as runner

COPY --from=builder /app/build/dieklingel-core*.deb /app/

RUN apt-get update && \
	apt-get install -y /app/dieklingel-core*.deb

ENTRYPOINT ["dieklingel-core"]
