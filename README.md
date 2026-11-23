# Microservicio de Benchmarking — C++ (`servicio-cpp-gt1`)

Microservicio desarrollado en **C++** como parte del proyecto global de comparación de rendimiento de algoritmos clásicos entre distintos lenguajes de programación.

## Requisitos previos

- Compilador compatible con **C++17** o superior (`g++` o `clang++`)
- **CMake** (versión 3.16 o superior) y `make` / `ninja`
- **Docker** (para ejecución en contenedor)
- **Git**

## Puesta en marcha en local

1. **Clonar el repositorio:**

```bash
   git clone https://github.com/I-FJ-I/servicio-cpp-gt1.git
   cd servicio-cpp-gt1
```

2. **Activar los hooks de Git:**

```bash
   git config core.hooksPath .githooks
```

3. **Configurar y compilar el proyecto con CMake:**

```bash
   cmake -B build -DCMAKE_BUILD_TYPE=Release
   cmake --build build
```

4. **Arrancar el servidor:**

```bash
   ./build/servicio-cpp-gt1
```

   *(Nota: verifica el nombre final del binario generado en `build/` según la configuración de `CMakeLists.txt`).*

El servicio quedará escuchando por defecto en `http://localhost:8080`.

## Contrato de la API

Todos los microservicios del proyecto global comparten el mismo contrato REST para permitir la integración uniforme con el frontend.

### Ejecutar benchmark

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

- [ ] `mergesort` (implementación base)
- [ ] `quicksort`
- [ ] `insertionsort`
- [ ] `selectionsort`
- [ ] `fibonacci`
- [ ] `matmul` (multiplicación de matrices)

## Ejecución de tests

Para compilar y lanzar la suite de tests unitarios y de integración mediante CTest:

```bash
cmake -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

## Despliegue con Docker

1. **Construir la imagen:**

```bash
   docker build -t servicio-cpp-gt1:latest .
```

2. **Ejecutar el contenedor:**

```bash
   docker run -d -p 8080:8080 --name servicio-cpp-gt1 servicio-cpp-gt1:latest
```
