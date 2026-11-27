#include "api/server.hpp"

namespace bench::api {

void register_routes(httplib::Server& server) {
  // Endpoint de salud: lo usan el HEALTHCHECK del Dockerfile, el smoke test
  // del pipeline y el despliegue para saber si el servicio está listo.
  server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
    res.set_content(R"({"status":"UP"})", "application/json");
  });

  // Cualquier ruta inexistente devuelve un JSON en vez de una página vacía
  server.set_error_handler([](const httplib::Request&, httplib::Response& res) {
    if (res.status == 404) {
      res.set_content(R"({"error":"Not Found"})", "application/json");
    }
  });
}

}  // namespace bench::api
