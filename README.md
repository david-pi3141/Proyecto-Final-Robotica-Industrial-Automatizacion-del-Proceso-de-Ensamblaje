
<div align="center">

<h3>Proyecto Final - Rob´otica Industrial</h3>

<h1>Automatización del Proceso de Ensamblaje, Soldadura y Empaque de
PCBs.</h1>

<img src="images/abbs.jpeg" alt="ABB" style="border-radius: 50%; width: 400px;"><br>

<b>Figura 1. </b>

</div>

---

Proyecto para la asignatura Robótica (2016770) Universidad Nacional de Colombia


# 1. Descripción del proyecto
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

# 2. Bitácora del desarrollo: decisiones, cambios, evidencias y resultados.

---

# 3. Descripción de la solución planteada.

---

# 4. Diseño del gripper/herramientas: planos + fotos + justificaci´on.

---

# 5. Diagrama de flujo del proceso


Modelos/simulaciones: RobotStudio (ABB), RoboDK (Yaskawa), EPSON RC+ (Epson).

---

# 6. Código fuente comentado: RAPID / Python / SPEL+.
Código fuente de utilizado para el desarrollo de la práctica.
## Descripción de las funciones utilizadas.

# 7. Visión de máquina

Para poder realizar la clasificación de los cuatro objetos, hicimos uso de visión de máquina. Inicialmente, se intentó hacer uso del modelo Net8 para realizar la clasificación, haciendo uso de un dataset que se encuentra disponible en https://www.kaggle.com/datasets/julioazancort/basic-electronic-components; sin el contenedor, tantas imágenes dificultaron el entrenamiento del modelo. Adicionalmente, se presentó el problema de que este dataset contiene imágenes donde el objeto a identificar ocupa la mayoría de la misma, situación que no se presenta en nuestro escenario.
 
Por lo tanto, se optó por cambiar el tipo de detección a emplear, decidiendo así usar YOLO en modo detector para realizar el entrenamiento.

Acompañado de esto, se tomaron fotos de cada uno de los objetos ubicados en el sitio. Durante este proceso observamos que la iluminación estaba afectando la calidad de la imagen, además del contraste que se produce en la misma. Para tratar de solucionar el problema de la iluminación, se probó colocar un objeto entre la fuente de luz y el sitio donde estaba el objeto, pero esto no dio muy buenos resultados. Lo segundo que se probó fue cambiar la configuración de la cámara, bajándole el brillo y un poco el contraste. Finalmente, una solución que se encontró fue dejar debajo del objeto una superficie blanca o clara grande, para que el contraste de la cámara permita distinguir el objeto. Cabe señalar que se tomaron dos “sesiones” de fotos, ya que tras las primeras se realizaron algunos cambios, por lo que para mantener el modelo lo más fiel a lo que va a observar la cámara fue necesario volver a tomar fotos.


<p align="center">
<img src="images/Roboflow.png" width="350">
<br>
<b>Figura. Datasets empleados para el entrenamiento. </b>
</p>

Luego de tomar las fotos, se procedió a etiquetarlas, para indicar a qué objeto corresponde cada foto, además de indicar la región de la imagen donde se encuentra, esto para evitar que tome parte del fondo para reconocer el objeto. Esta tarea se realizó en la plataforma Roboflow. El primer dataset contiene en total 87 imágenes, y el segundo contiene 174 imágenes. Luego se aplica data augmentation para darle más variedad al dataset. Este dataset se descarga para luego usarlo de manera local para el entrenamiento.

Tras realizar el entrenamiento, se aplicó el modelo de visión de máquina para identificar los cuatro objetos haciendo uso de una webcam conectada por USB a la PC. En este programa, inicialmente debemos iniciar la comunicación con el puerto serial COM, en este caso COM3; esto se hace con el fin de poderse comunicar con el Arduino Uno, tema que se tratará más adelante. Luego cargamos el modelo anteriormente entrenado e iniciamos la cámara. A lo largo del programa se emiten algunos mensajes para confirmar la comunicación entre los dispositivos. El programa se queda esperando recibir el mensaje "IR" desde el puerto serial para luego realizar una captura de la cámara, realizar el análisis con el modelo, mostrar en pantalla la captura tomada junto con la etiqueta y la caja que envuelve al objeto y, finalmente, emitir un mensaje con el resultado hacia el puerto serial. Adicionalmente, en pantalla se verá la confiabilidad que se tiene de la detección.

---

## Descripción de las funciones utilizadas.

### Python/YOLO

En el programa train_det.py se maneja únicamente una función, la cual a su vez hace uso de un método para realizar el entrenamiento, el cual lo ofrece Ultralytics. Este método recibe como argumentos data, epochs, imgsz, batch, workers, amp, pretrained, cache, project, name. Este método pertenece al objeto model que se crea al inicio de la función, al cual se le asigna el modelo del cual va a partir para realizar el entrenamiento, también llamado finetuning. El primer entrenamiento realizado usó YOLOv26m como punto de partida. Además, este se realizó haciendo 30 "repeticiones" o epochs, redimensionando las imágenes a 640 píxeles, procesando ocho imágenes simultáneamente por paso de entrenamiento, aceptando los pesos del modelo preentrenado y activando el entrenamiento con precisión mixta automática, la cual permite reducir el uso de la memoria de la GPU, ya que se está usando CUDA. Luego de entrenarlo, se tomó el modelo entrenado y se reentrenó usando el segundo dataset. En los anexos se puede consultar el archivo con el que se entrenó.

### Arduino



---

# 8. Plano de planta

---

# 9. Esquemático de conexiones

---


# 10. Video de simulación

---

# 11. Video de implementación física

---

# 12. Conclusiones

---

# Referencias
