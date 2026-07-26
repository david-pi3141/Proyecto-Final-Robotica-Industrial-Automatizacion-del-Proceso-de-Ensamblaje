
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

---

# Bitácora del desarrollo: decisiones, cambios, evidencias y resultados.

---

# Diagramas de flujo del proceso


Modelos/simulaciones: RobotStudio (ABB), RoboDK (Yaskawa), EPSON RC+ (Epson).

---

# Comparación manual vs automatizado: tiempos por PCB, tasa de fallos, repetibilidad.

Descripción detallada de la solución planteada.

Diagrama de flujo de acciones del robot.

---

# Plano de planta

---

# Diseño de la herramienta
Diseño del gripper/herramientas: planos + fotos + justificaci´on.

---

# Código fuente comentado: RAPID / Python / SPEL+.
Código fuente de utilizado para el desarrollo de la práctica.
## Descripción de las funciones utilizadas.

## Visión de máquina

Para poder realizar la clasificación de los cuatro objetos, hicimos uso de visión de máquina. Inicialmente, se intentó hacer uso del modelo Net8 para realizar la clasificación, haciendo uso de un dataset que se encuentra disponible en https://www.kaggle.com/datasets/julioazancort/basic-electronic-components; sin el contenedor, tantas imágenes dificultaron el entrenamiento del modelo. Adicionalmente, se presentó el problema de que este dataset contiene imágenes donde el objeto a identificar ocupa la mayoría de la misma, situación que no se presenta en nuestro escenario.
 
Por lo tanto, se optó por cambiar el tipo de detección a emplear, decidiendo así usar YOLO en modo detector para realizar el entrenamiento.

Acompañado de esto, se tomaron fotos de cada uno de los objetos ubicados en el sitio. Durante este proceso observamos que la iluminación estaba afectando la calidad de la imagen, además del contraste que se produce en la misma. Para tratar de solucionar el problema de la iluminación, se probó colocar un objeto entre la fuente de luz y el sitio donde estaba el objeto, pero esto no dio muy buenos resultados. Lo segundo que se probó fue cambiar la configuración de la cámara, bajándole el brillo y un poco el contraste. Finalmente, una solución que se encontró fue dejar debajo del objeto una superficie blanca o clara grande, para que el contraste de la cámara permita distinguir el objeto. Cabe señalar que se tomaron dos “sesiones” de fotos, ya que tras las primeras se realizaron algunos cambios, por lo que para mantener el modelo lo más fiel a lo que va a observar la cámara fue necesario volver a tomar fotos.


<p align="center">
<img src="images/Roboflow.png" width="350">
<br>
<b>Figura. Datasets empleados para el entrenamiento. </b>
</p>

Luego de tomar las fotos, se procedió a etiquetarlas, para indicar a qué objeto corresponde cada foto, además de indicar la región de la imagen donde se encuentra, esto para evitar que tome parte del fondo para reconocer el objeto. Esta tarea se realizó en la plataforma Roboflow. El primer dataset contiene en total 87 imágenes, y el segundo contiene 174 imágenes. Luego se aplica data augmentation para darle más variedad al dataset. Este dataset se descarga para luego usarlo de manera local para el entrenamiento.


Ahora, dese un script de Python que se entrena usando YOLOv26m. Se realizaron dos entrenamientos: el primero a partir de YOLOv26m y el primer dataset; luego de entrenarlo, se tomó el modelo entrenado y se reentrenó usando el segundo dataset.

---

# Video del funcionamiento del proyecto

---

# Conclusiones

---

# Referencias
