# Guía de Contribución — `servicio-cpp-gt1`

Gracias por contribuir al microservicio en C++ (`servicio-cpp-gt1`). Para que tu Pull Request pueda ser revisado, aceptado y evaluado correctamente, sigue estrictamente estas normas.

## 1. Configuración del entorno y Git

1. **Identidad en Git (obligatorio):** Todos los commits deben estar firmados con tu nombre real y correo electrónico. No se aceptarán commits atribuidos a herramientas de IA.

```bash
   git config user.name "Tu Nombre y Apellidos"
   git config user.email "tu.email@ejemplo.com"
```

2. **Sincronización limpia:** Configura `pull.ff only` para evitar commits de fusión automáticos innecesarios:

```bash
   git config --global pull.ff only
```

3. **Hooks de Git:** Activa los hooks del repositorio antes de hacer tu primer commit:

```bash
   git config core.hooksPath .githooks
```

4. **Entorno C++:** Asegúrate de contar con un compilador compatible con C++17 o superior (`g++`/`clang++`) y **CMake** (3.16+) para compilar y ejecutar los tests en local.

## 2. Flujo de trabajo (Fork + Pull Request)

Como colaborador externo al equipo propietario de `servicio-cpp-gt1`, debes trabajar desde un **fork**:

1. **Elige un Issue:** Revisa la pestaña **Issues** y comenta en el algoritmo que vas a implementar para evitar duplicar trabajo. Solo se aceptan contribuciones de código no triviales (editar documentación no es válido).
2. **Haz Fork y clona tu copia:**

```bash
   git clone https://github.com/I-FJ-I/servicio-cpp-gt1.git
   cd servicio-cpp-gt1
   git remote add upstream https://github.com/I-FJ-I/servicio-cpp-gt1.git
```

3. **Crea una rama desde `main`:**

```bash
   git switch main
   git pull --ff-only upstream main
   git switch -c feat/<nombre-algoritmo>
```

4. **Implementa y verifica en local:** Compila el proyecto con CMake y comprueba que todos los tests pasan antes de subir tus cambios:

```bash
   cmake -B build -DBUILD_TESTING=ON
   cmake --build build
   ctest --test-dir build --output-on-failure
```

5. **Sube tu rama y abre un Pull Request:**

```bash
   git push -u origin feat/<nombre-algoritmo>
```

- Rellena todas las secciones de la plantilla del Pull Request.
- Enlaza el issue correspondiente escribiendo `Closes #<numero-del-issue>` en la descripción.

## 3. Requisitos técnicos de los algoritmos en C++

Cualquier algoritmo nuevo debe integrarse en el endpoint `POST /api/v1/benchmark` respetando el contrato común (`"language": "cpp"`) y cumpliendo estas reglas:

- **Protocolo de medición:** Se deben ejecutar **5 mediciones** independientes (usando un reloj monótono de alta precisión como `std::chrono::steady_clock`) y devolver la **mediana** del tiempo en milisegundos (`timeMs`), con `"runs": 5`.
- **Algoritmos de ordenación (`quicksort`, `insertionsort`, `selectionsort`):** Deben generar sus vectores de entrada de forma aleatoria en cada ejecución (por ejemplo, empleando `<random>` con `std::mt19937`).
- **Fibonacci (`fibonacci`):** Debe implementarse mediante **programación dinámica** (enfoque iterativo/tabular).
- **Multiplicación de matrices (`matmul`):** Debe operar sobre matrices cuadradas de dimensiones `size x size` según el parámetro `size` recibido en la petición.

## 4. Tests y calidad del código

- **Tests unitarios:** Todo PR debe añadir tests unitarios en C++ (integrados en CTest) que verifiquen que el algoritmo produce resultados exactos (por ejemplo, comprobando con `std::is_sorted` que el vector queda ordenado).
- **Tests de integración:** El endpoint `POST /api/v1/benchmark` debe responder correctamente con el nuevo algoritmo sin romper los endpoints ni algoritmos existentes.
- **Uso de IA:** Está permitido apoyarse en asistentes de IA, pero debes revisar, comprender y validar todo el código C++ y su gestión de memoria antes de abrir el PR.

## 5. Revisión y política de fusión

- **Pipeline de CI:** El PR debe superar todos los checks automáticos de compilación y tests en verde.
- **Revisión de Code Owners:** Al menos un miembro del equipo propietario de `servicio-cpp-gt1` debe revisar y aprobar el PR, y todas las conversaciones deben quedar resueltas.
- **Estrategia de merge:** Este repositorio utiliza **Squash and merge** para mantener un historial lineal y limpio en `main` donde cada Pull Request equivale a un único commit con significado.
