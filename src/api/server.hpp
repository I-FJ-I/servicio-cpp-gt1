#pragma once

#include <httplib.h>

namespace bench::api {

// Registra en `server` todas las rutas de la API REST del microservicio.
void register_routes(httplib::Server& server);

}  // namespace bench::api
