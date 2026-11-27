# Microservicio de Benchmarking — C++ (`servicio-cpp-gt1`)

Microservicio desarrollado en **C++** como parte del proyecto global de comparación de rendimiento de algoritmos clásicos entre distintos lenguajes de programación.

## Requisitos previos

Hay dos formas de preparar el entorno de desarrollo:

**Opción A — Dev Container (recomendada, no hay que instalar nada más):**

- **Docker** (Docker Desktop en Windows/macOS)
- **VS Code** con la extensión **Dev Containers**

**Opción B — Instalación local (Linux o WSL2):**

- Compilador compatible con **C++17** o superior (`g++` o `clang++`)
- **CMake 3.21** o superior y **Ninja** (o `make`)
- **clang-format 21.1.8** (`pip install clang-format==21.1.8`)
- **Git**

Las dependencias C++ (cpp-httplib y GoogleTest) las descarga CMake automáticamente con versión y hash fijados.

## Puesta en marcha en local

1. **Clonar el repositorio:**

```bash
git clone https://github.com/I-FJ-I/servicio-cpp-gt1.git
cd servicio-cpp-gt1
```

2. **Preparar el entorno:**

   - **Con Dev Container:** abre la carpeta en VS Code y ejecuta *Dev Containers: Reopen in Container*. El contenedor trae GCC, CMake, Ninja y clang-format, activa los hooks de Git y configura CMake en `build/` automáticamente.
   - **En local:** activa los hooks de Git (una vez por clon):

```bash
git config core.hooksPath .githooks
```

3. **Configurar y compilar con CMake:**

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

4. **Arrancar el servidor:**

```bash
./build/servicio-cpp-gt1
```

El servicio queda escuchando en `http://localhost:8080`. El puerto se puede cambiar con la variable de entorno `PORT` (por ejemplo, `PORT=9090 ./build/servicio-cpp-gt1`).

5. **Comprobar que está vivo:**

```bash
curl http://localhost:8080/health
# {"status":"UP"}
```

## Contrato de la API

Todos los microservicios del proyecto global comparten el mismo contrato REST para permitir la integración uniforme con el frontend.

### Salud del servicio

- **Endpoint:** `GET /health`
- **Respuesta (`200 OK`):** `{"status":"UP"}`

Lo usan el `HEALTHCHECK` de la imagen Docker y el pipeline para saber si el servicio está listo. Cualquier ruta inexistente devuelve `404` con `{"error":"Not Found"}`.

### Ejecutar benchmark

> Pendiente de implementar: el endpoint todavía no está disponible.

- **Endpoint:** `POST /api/v1/benchmark`
- **Headers:** `Content-Type: application/json`

**Ejemplo de petición:**

```json
{
  "algorithm": "mergesort",
  "size": 10000
}
```

**Ejemplo de respuesta (`200 OK`):**

```json
{
  "language": "cpp",
  "algorithm": "mergesort",
  "size": 10000,
  "timeMs": 12.34,
  "runs": 5
}
```

> **Nota sobre la medición:** Cada petición ejecuta el algoritmo **5 veces** de forma independiente y devuelve en `timeMs` la **mediana** de las mediciones en milisegundos.

### Algoritmos disponibles

- [ ] `mergesort` (implementación base; algoritmo y tests listos, pendiente de exponerlo en el endpoint)
- [ ] `quicksort`
- [ ] `insertionsort`
- [ ] `selectionsort`
- [ ] `fibonacci`
- [ ] `matmul` (multiplicación de matrices)

## Estructura del proyecto

```text
src/
├── algorithms/   # Implementaciones de los algoritmos (sin dependencias)
├── api/          # Rutas de la API REST (cpp-httplib)
└── main.cpp      # Punto de entrada: lee PORT y arranca el servidor
tests/
├── test_mergesort.cpp   # Tests unitarios (etiqueta "unit")
└── test_api.cpp         # Tests de integración de los endpoints (etiqueta "integration")
```

## Ejecución de tests

Para compilar y lanzar la suite de tests unitarios y de integración con CTest:

```bash
cmake -B build -G Ninja -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Para lanzar solo un tipo de tests: `ctest --test-dir build -L unit` o `ctest --test-dir build -L integration`.

## Formato del código

El código sigue el estilo de Google, definido en `.clang-format`. El hook de pre-commit formatea automáticamente los ficheros C++ de cada commit. Para comprobarlo o aplicarlo a mano:

```bash
git ls-files '*.cpp' '*.hpp' | xargs clang-format --dry-run --Werror   # comprobar
git ls-files '*.cpp' '*.hpp' | xargs clang-format -i                   # formatear
```

## Despliegue con Docker

> Disponible cuando se complete el issue #3 (Dockerfile y pipeline de CI/CD).

1. **Construir la imagen:**

```bash
docker build -t servicio-cpp-gt1:latest .
```

2. **Ejecutar el contenedor:**

```bash
docker run -d -p 8080:8080 --name servicio-cpp-gt1 servicio-cpp-gt1:latest
```
