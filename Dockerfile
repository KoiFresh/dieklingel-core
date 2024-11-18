FROM debian:bookworm AS builder

WORKDIR /app

COPY . .

RUN apt-get update && \
	apt-get install -y make && \
	make deps:all

RUN make clean package RASPBERRYPI_LIBCAMERA_SUPPORT="ON" NO_LINT="ON"

FROM debian:bookworm AS runner

COPY --from=builder /app/build/dieklingel-core*.deb /app/

RUN apt-get update && \
	apt-get install -y /app/dieklingel-core*.deb

ENTRYPOINT ["dieklingel-core"]
