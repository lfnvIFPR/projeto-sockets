add_executable("server" server.c serverMain.c stringDinamica.c utilidade.c vector.c)
add_executable("cliente" cliente.c stringDinamica.c vector.c utilidade.c)

set_target_properties("server" PROPERTIES
  C_STANDARD 11
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF
)

configure_file(index.html index.html COPYONLY)
configure_file(not_found.html not_found.html COPYONLY)
configure_file(doido.html doido.html COPYONLY)
configure_file(icon.png icon.png COPYONLY)
