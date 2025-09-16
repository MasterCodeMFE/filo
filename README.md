# filo

## 🧑‍💻 Proyecto: Filósofos (Dining Philosophers)

Este proyecto es una implementación profesional y robusta del clásico problema de los **Filósofos Comensales** (Dining Philosophers), desarrollado en C y orientado a la gestión eficiente de concurrencia y sincronización de hilos usando POSIX threads (pthreads).

---

## 🚀 Descripción General

El objetivo es simular el comportamiento de varios filósofos sentados alrededor de una mesa, alternando entre pensar, comer y dormir, mientras comparten recursos limitados (tenedores) y evitando condiciones de carrera y deadlocks.

- **Lenguaje:** C (C99)
- **Librerías:** pthread, stdio, stdlib, unistd, sys/time
- **Paradigma:** Programación concurrente y sincronización con mutex
- **Compilación:** Makefile profesional

---

## 📂 Estructura del Proyecto

```
filo/
├── filo.h              # Header principal
├── filo_struc.h        # Definición de la estructura principal
├── Makefile            # Compilación y gestión de dependencias
├── src/                # Código fuente modularizado
│   ├── main.c
│   ├── create_threads.c
│   ├── eat.c
│   ├── ft_atoi.c
│   ├── ft_usleep.c
│   ├── init_mutex.c
│   ├── monitoring.c
│   ├── print_sleep.c
│   ├── routine.c
│   ├── sleep.c
│   ├── take_fork.c
│   ├── think.c
│   └── time.c
└── obj/                # Objetos compilados
```

---

## 🏗️ Compilación

Para compilar el proyecto, simplemente ejecuta:

```sh
make
```
Esto generará el ejecutable `philo` en el directorio raíz.

Para limpiar archivos objeto y binarios:
```sh
make clean   # Elimina archivos objeto
make fclean  # Elimina objetos y ejecutable
make re      # Limpia y recompila
```

---

## ⚙️ Ejecución

El programa se ejecuta desde la terminal:

```sh
./philo <n_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [n_times_each_must_eat]
```

- `<n_philosophers>`: Número de filósofos (y tenedores)
- `<time_to_die>`: Tiempo (ms) antes de que un filósofo muera si no come
- `<time_to_eat>`: Tiempo (ms) que tarda un filósofo en comer
- `<time_to_sleep>`: Tiempo (ms) que tarda un filósofo en dormir
- `[n_times_each_must_eat]`: (Opcional) Número de veces que cada filósofo debe comer antes de terminar

**Ejemplo:**
```sh
./philo 5 800 200 200
```

---

## 🧩 Arquitectura y Diseño

- **Sincronización:** Uso intensivo de mutex para proteger recursos compartidos (tenedores, impresión, estados).
- **Monitorización:** Hilo dedicado para detectar muertes y finalización de la simulación.
- **Prevención de deadlocks:** Estrategias de alternancia y control de acceso a los tenedores.
- **Mensajes enriquecidos:** Salida con emojis para visualizar el estado de cada filósofo (comiendo, pensando, durmiendo, tomando tenedor, muerte).
- **Gestión de errores:** Validación exhaustiva de argumentos y recursos.

---

## 📝 Ejemplo de Salida

```
🍴 1 2 has taken a fork (left) 🍴
🍴 1 2 has taken a fork (right) 🍴
🍽️  1 2 is eating 🍽️
🛌 201 2 is sleeping 🛌
🧑 401 2 is thinking 🧑
💀 800 2 has died 💀
```

---

## 📚 Archivos Clave

- **filo.h / filo_struc.h:** Definición de la estructura principal y prototipos.
- **main.c:** Inicialización, parseo de argumentos y ciclo principal.
- **create_threads.c:** Creación y gestión de hilos de filósofos y monitor.
- **monitoring.c:** Detección de muerte y finalización.
- **take_fork.c / eat.c / sleep.c / think.c:** Acciones principales de los filósofos.
- **ft_atoi.c / ft_usleep.c / time.c:** Utilidades auxiliares.

---

## 🏅 Características Profesionales

- Código modular, limpio y documentado.
- Manejo seguro de memoria y recursos.
- Compatible con sistemas Unix/Linux.
- Makefile robusto y portable.
- Salida visual amigable y profesional.

---

## 👨‍💻 Autor

- **Manuel Fernández**  
  [manufern@student.42.fr](mailto:manufern@student.42.fr)

---

## 📜 Licencia

Este proyecto es parte del currículo de 42 Madrid y se distribuye bajo licencia académica.
