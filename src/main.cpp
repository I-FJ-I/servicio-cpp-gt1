#include <httplib.h>
#include <signal.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>

#include "api/server.hpp"

namespace {

constexpr int kDefaultPort = 8080;

// Lee el puerto de la variable de entorno PORT (8080 si no está definida).
int read_port() {
  const char* value = std::getenv("PORT");
  if (value == nullptr || *value == '\0') return kDefaultPort;
  try {
    const int port = std::stoi(value);
    if (port > 0 && port <= 65535) return port;
  } catch (const std::exception&) {
  }
  std::cerr << "PORT no válido ('" << value << "'), se usa " << kDefaultPort
            << '\n';
  return kDefaultPort;
}

}  // namespace

int main() {
  // Bloquea SIGINT/SIGTERM en todos los hilos y los atiende en uno dedicado:
  // así `docker stop` (SIGTERM) para el servidor de forma ordenada.
  sigset_t signals;
  sigemptyset(&signals);
  sigaddset(&signals, SIGINT);
  sigaddset(&signals, SIGTERM);
  pthread_sigmask(SIG_BLOCK, &signals, nullptr);

  httplib::Server server;
  bench::api::register_routes(server);

  std::thread signal_waiter([&server, signals]() {
    int received = 0;
    sigwait(&signals, &received);
    std::cout << "Señal " << received << " recibida, parando el servidor\n";
    server.stop();
  });
  signal_waiter.detach();

  const int port = read_port();
  std::cout << "servicio-cpp-gt1 escuchando en http://0.0.0.0:" << port
            << std::endl;
  if (!server.listen("0.0.0.0", port)) {
    std::cerr << "No se pudo escuchar en el puerto " << port << '\n';
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
