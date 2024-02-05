set(SERVICE_NAME dieklingel-core.service)
set(EXE_PATH ${CMAKE_INSTALL_PREFIX}/bin/dieklingel-core)
set(SERVICE_IN ${SERVICE_NAME}.in)
set(SERVICE_OUT ${CMAKE_CURRENT_BINARY_DIR}/${SERVICE_NAME})
configure_file(${SERVICE_IN} ${SERVICE_OUT} @ONLY)

install(
	FILES ${SERVICE_OUT}
	DESTINATION /etc/systemd/user
)
