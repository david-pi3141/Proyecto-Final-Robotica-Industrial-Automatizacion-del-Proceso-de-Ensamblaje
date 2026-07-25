
<div align="center">

<h3>Proyecto Final - Rob´otica Industrial</h3>

<h1>Automatización del Proceso de Ensamblaje, Soldadura y Empaque de
PCBs.</h1>

<img src="images/abbs.jpeg" alt="ABB" style="border-radius: 50%; width: 400px;"><br>

<b>Figura 1. </b>

</div>

---

Proyecto para la asignatura Robótica (2016770) Universidad Nacional de Colombia


# Descripción de ´rpyecto
Recepción, clasificación y ordenamiento (ABB IRB 140 “Caín”)

Objetivo: recibir componentes en desorden desde banda, identificarlos (tipo y/o tamaño) y organizarlos en un almacén ordenado por categorías para el ensamblaje. Entrada:

Componentes mezclados llegando por banda.

Almacén vacío/semivacío con posiciones asignadas por tipo.
Secuencia mínima requerida:

1. Inicialización: home + verificación de estado seguro.
2. Detección en banda (visi´on opcional):
Detectar un componente disponible en el área de pick (posición aproximada).
(Opcional) identificar tipo con visión/clasificación simple.
3. Pick en banda: approach → pick → levantar.
4. Clasificación: decidir a qu´e zona del almac´en pertenece (por tipo de componente).
5. Place en almacén ordenado: depositar en la celda correspondiente (por ejemplo: “Resistencias”, “Capacitores”, “CI”, “Conectores”).
6. Conteo/Inventario: mantener conteo por tipo para garantizar que haya ¿= 30 componentes listos para
el Epson.
7. Fin de etapa: señal “Almacén listo” cuando se alcance el conteo requerido (según receta).

Verificación mínima:

Confirmar pick (sensor/tiempo/confirmación por operador).
 
Confirmar que el componente fue depositado en la celda correcta (por conteo o visión puntual).

Salida:
Almacén organizado con componentes clasificados y listos para ensamblaje (mínimo 30 para 1 PCB).

Fallas típicas (manejo obligatorio):

No hay componente en el área de pick → esperar/reintentar.
Pick fallido → reintentar hasta N veces y luego alarma.
Clasificación incierta → enviar a bandeja “Rechazo” o pedir confirmación en HMI.

# Bitácora del desarrollo: decisiones, cambios, evidencias y resultados.

# Diagramas de flujo del proceso


Modelos/simulaciones: RobotStudio (ABB), RoboDK (Yaskawa), EPSON RC+ (Epson).



# Comparación manual vs automatizado: tiempos por PCB, tasa de fallos, repetibilidad.

Descripción detallada de la solución planteada.

Diagrama de flujo de acciones del robot.

# Plano de planta



# Diseño de la herramienta
Diseño del gripper/herramientas: planos + fotos + justificaci´on.

# Código fuente comentado: RAPID / Python / SPEL+.
Código fuente de utilizado para el desarrollo de la práctica.
## Descripción de las funciones utilizadas.

# Video del funcionamiento del proyecto

# Conclusiones

# Referencias
