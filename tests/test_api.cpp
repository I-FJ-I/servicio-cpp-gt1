#include <gtest/gtest.h>
#include <httplib.h>

#include <thread>

#include "api/server.hpp"

namespace {

// Arranca el servidor real con todas las rutas en un puerto libre de
// 127.0.0.1 y lo para al terminar cada test.
class ApiTest : public ::testing::Test {
 protected:
  void SetUp() override {
    bench::api::register_routes(server_);
    port_ = server_.bind_to_any_port("127.0.0.1");
    ASSERT_GT(port_, 0) << "No se pudo reservar un puerto para el servidor";
    thread_ = std::thread([this]() { server_.listen_after_bind(); });
    server_.wait_until_ready();
  }

  void TearDown() override {
    server_.stop();
    if (thread_.joinable()) thread_.join();
  }

  httplib::Client client() const { return httplib::Client("127.0.0.1", port_); }

 private:
  httplib::Server server_;
  std::thread thread_;
  int port_ = -1;
};

}  // namespace

TEST_F(ApiTest, HealthReturnsUp) {
  auto res = client().Get("/health");

  ASSERT_TRUE(res) << "Sin respuesta del servidor";
  EXPECT_EQ(res->status, 200);
  EXPECT_EQ(res->get_header_value("Content-Type"), "application/json");
  EXPECT_EQ(res->body, R"({"status":"UP"})");
}

TEST_F(ApiTest, UnknownRouteReturnsJson404) {
  auto res = client().Get("/no-existe");

  ASSERT_TRUE(res) << "Sin respuesta del servidor";
  EXPECT_EQ(res->status, 404);
  EXPECT_EQ(res->get_header_value("Content-Type"), "application/json");
  EXPECT_EQ(res->body, R"({"error":"Not Found"})");
}
