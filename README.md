
<div align="center">

<h3>Proyecto Final - Robótica Industrial</h3>

<h1>Automatización del Proceso de Ensamblaje, Soldadura y Empaque de
PCBs.</h1>

<img src="images/Foto.jpeg" alt="ABB" style="border-radius: 50%; width: 400px;"><br>

<b>Figura 1. </b>

</div>

---

Proyecto para la asignatura Robótica (2016770) Universidad Nacional de Colombia


# 1. Descripción del proyecto

## Contexto general

El proyecto tiene como objetivo general el desarrollo de una línea automatizada para el ensamblaje de circuitos impresos, PCB. Esta línea está conformada por 4 estaciones robotizadas, en donde cada una tiene un objetivo específico. De esta manera, se tienen cuatro estaciones dispuestas de manera secuencial que corresponden a la clasificación, ensamblaje, soldadura y embalaje de las placas. Cada estación es operada por un robot industrial diferente y cumple una función específica dentro del proceso de manufactura.

La presente descripción corresponde específicamente al desarrollo de la primera parte del proyecto que será desarrollada con el robot ABB IRB 140 “Caín”, el cual estará encargado de recibir componentes electrónicos transportados por una banda, clasificarlos de acuerdo con su tipo y organizarlos en un almacén estructurado para facilitar el proceso de ensamblaje realizado posteriormente por el robot Epson.

## Objetivo de la estación

La primera etapa de la línea automatizada de ensamblaje de circuitos impresos (PCB) corresponde a la recepción, clasificación y almacenamiento de componentes electrónicos mediante el robot ABB IRB 140 "Caín". El propósito de esta estación es recibir componentes que llegan de forma desordenada sobre una banda transportadora, identificarlos según su tipo o categoría y organizarlos en un almacén inicialmente vacío o parcialmente ocupado, cuyas posiciones se encuentran previamente asignadas para cada categoría de componente. Esta organización permite que la siguiente estación del proceso disponga de los componentes ordenados para realizar el ensamblaje de la PCB de manera eficiente y repetitiva.

## Secuencia general del proceso

El proceso inicia con la inicialización del sistema, donde el robot debe desplazarse a la posición de Home y verificar que se cumplen las condiciones necesarias para comenzar la operación.

Posteriormente, el sistema debe detectar la presencia de un componente en el área de recolección de la banda transportadora. De manera opcional, puede emplearse un sistema de visión para identificar el tipo o tamaño del componente antes de su manipulación.

Una vez detectado el componente, el robot ejecuta una operación de Pick, realizando una aproximación segura, la toma del componente y su posterior elevación. Con base en la clasificación obtenida, el sistema determina la categoría a la que pertenece el componente y selecciona la posición correspondiente dentro del almacén.

Finalmente, el robot realiza la operación de Place, depositando el componente en la celda asignada. Este procedimiento se repite hasta completar la cantidad de componentes requerida para abastecer la estación de ensamblaje. Durante todo el proceso, el sistema mantiene un conteo del inventario clasificado y genera una señal de finalización cuando se alcanza la cantidad establecida para la receta de producción.

## Verificaciones necesarias

Para garantizar el correcto desarrollo del proceso, la estación debe incorporar mecanismos de verificación durante las operaciones críticas. En primer lugar, se debe confirmar que la operación de Pick fue realizada exitosamente, ya sea mediante sensores, temporización o confirmación del operador. De igual manera, debe verificarse que el componente fue depositado en la posición correcta del almacén, utilizando un conteo de inventario o un sistema de visión cuando se encuentre disponible.

## Resultado

El sistema debe ser capaz de entregar un almacén organizado con los componentes clasificados por categorías y disponibles para la siguiente etapa de ensamblaje. El inventario debe contener la cantidad de componentes requerida para completar el ensamblaje de una PCB, garantizando que los elementos se encuentren correctamente distribuidos según su tipo.

## Manejo mínimo de fallas típicas

Durante la operación pueden presentarse diferentes situaciones que deben ser consideradas dentro del diseño del sistema. Entre ellas se encuentran la ausencia de componentes en el área de recolección, lo que obliga al sistema a esperar o reintentar la detección; un fallo durante la operación de Pick, que requiere realizar nuevos intentos antes de generar una condición de alarma; y una clasificación incierta del componente, caso en el cual este puede enviarse a una bandeja de rechazo o solicitar la intervención del operador mediante la interfaz HMI para confirmar la clasificación.

---

# 2. Bitácora del desarrollo: decisiones, cambios, evidencias y resultados.

## Arquitectura de comunicación

Originalmente, la idea para conseguir que el robot fuese capaz de actuar en consecuencia a un reconocimiento de objetos era usar un servidor de Ignition, la cual es una plataforma de software para automatización industrial. Este servidor permitiría conectar un modelo de visión de máquina a un IOT gateway de Robot Studio. Este modelo sería programado y ejecutado en Matlab usando imágenes propias tomadas con la cámara a utilizar en el espacio de trabajo. La intención de hacer esto era que el modelo fuese capaz de hacer la clasificación y, adicionalmente, entregar la posición del objeto respecto a la cámara para luego encontrar la posición del objeto respecto al robot.

<p align="center">
<img src="images/arquitectura_inicial.png" width="350">
<br>
<b>Figura. Primera arquitectura planteada. </b>
</p>

Luego de tener la posición respecto al robot, el programa en RAPID que corre el controlador IRC5 recibiría esta clasificación y posición para adaptar sus movimientos con el fin de que el gripper (electroimán) recogiera el objeto en el punto exacto donde se encuentra. Evitando así otros movimientos adicionales.

Sin embargo, por cuestiones de experiencia con este tipo de implementación, y por cuestiones de tiempo, se descartó esta idea, optando por usar como método de comunicación entre el modelo y el controlador del robot un Arduino. En este nuevo escenario, lo que se busca es que cuatro salidas digitales representen cada una de las cuatro clases que debe identificar el robot. De este modo, el controlador recibirá cuatro entradas digitales que debe leer para ejecutar un procedimiento adecuado según la clase que represente cada entrada.

Por otro lado, se decidió hacer uso de Python en lugar de MATLAB, dado que ya existía una experiencia previa con su uso para este tipo de aplicación.

## Soporte cámara
## Diseño gripper
## Conexiones sistema de control y potencia
## Fuente de alimentación
## Vision de maquina

Inicialmente, se intentó hacer uso del modelo Net8 para realizar la clasificación, haciendo uso de un dataset que se encuentra disponible en [Dataset](https://www.kaggle.com/datasets/julioazancort/basic-electronic-components), el cual contiene aproximadamente 8000 imágenes, clasificadas en cinco clases: Capacitor, Dataset_Treino, IC, Resistor, Transistor. Aunque con imágenes similares el modelo detectó correctamente la clase, al probarlo con las imágenes de nuestro escenario real, realizó mal la clasificación.

<p align="center">
<img src="images/Primer_Dat.png" width="350">
<br>
<b>Figura. Etiquetado del primer Dataset. </b>
</p>

En consecuencia, se intentó realizar un etiquetado para intentar usar este primer dataset con YOLO, haciendo uso de un script de Python, el cual asignó una etiqueta automáticamente según la clase. Sin embargo, tantas imágenes dificultaron el entrenamiento del modelo. Adicionalmente, se presentó el problema de que este dataset contiene imágenes donde el objeto a identificar ocupa la mayoría de la misma, situación que no se presenta en nuestro escenario. Este problema provocó que, al intentar realizar reentrenamiento del modelo "yolo11x-cls.pt", este tomase parte del fondo como información importante para reconocer la clase de la imagen. Por lo tanto, se optó por cambiar el tipo de detección a emplear, decidiendo así usar YOLO en modo detector para realizar el entrenamiento.

<div align="center">
  <table>
    <!-- Primera Fila -->
    <tr>
      <td align="center">
        <img src="images/Prueba_prim.jpg" width="300"><br>
        <b>(a)</b>
      </td>
      <td align="center">
        <img src="images/Matriz_conf_prim.png" width="300"><br>
        <b>(b)</b>
      </td>
    </tr>
  </table>
  <br>
  <b>Figura . </b> Entrenamiento con yolo11x: (a) Imágen del entrenamiento y (b) Matriz de confusión.
</div>

En la imagen que se muestra arriba, se puede observar cómo el entrenamiento tomó una imagen sin ningún objeto como parte de una clase; además, en la imagen que se encuentra al lado, se observa la matriz de confusión que se genera de forma automática al usar YOLO. Esta matriz nos muestra que únicamente está reconociendo los objetos nulos, y los reconoce como conectores. Independientemente de la interpretación del resultado en particular, la conclusión principal es que no está realizando correctamente el entrenamiento. Probablemente por dos razones. La primera es por errores en el etiquetado, ya que se hizo de forma automática, y la segunda por el tipo de imagen que se está usando. 

<p align="center">
<img src="images/Roboflow.png" width="350">
<br>
<b>Figura. Datasets empleados para el entrenamiento. </b>
</p>

Acompañado de esto, se tomaron fotos de cada uno de los objetos ubicados en el sitio. Durante este proceso observamos que la iluminación estaba afectando la calidad de la imagen, además del contraste que se produce en la misma. Para tratar de solucionar el problema de la iluminación, se probó colocar un objeto entre la fuente de luz y el sitio donde estaba el objeto, pero esto no dio muy buenos resultados. Lo segundo que se probó fue cambiar la configuración de la cámara, bajándole el brillo y un poco el contraste. Finalmente, una solución que se encontró fue dejar debajo del objeto una superficie blanca o clara grande, para que el contraste de la cámara permita distinguir el objeto.

## Detección de componente

---

# 3. Descripción de la solución planteada.

---

# 4. Diseño del gripper/herramientas: planos + fotos + justificaci´on.

---

# 5. Diagramas de flujo del proceso

Una vez definido el funcionamiento del sistema, se presentan los diagramas de flujo correspondientes a las rutinas y subprocesos que conforman el proceso de clasificación y almacenamiento de componentes. Debido a que el programa está estructurado de forma modular y contiene diversos procedimientos, además del diagrama de flujo principal se incluyen los diagramas de las subrutinas más relevantes para facilitar la comprensión de la lógica implementada. En el diagrama principal, dichas subrutinas se resaltan mediante bloques de color verde.

Nota: En el bloque "Esperar componente" se considera que la banda transportadora permanece en funcionamiento hasta que el sensor detecta la llegada de un componente, momento en el cual la banda se detiene para continuar con el proceso.

```mermaid
flowchart TD

    A([Inicio])

    B[Inicializar sistema]
    C[Esperar botón Inicio]

    D{¿Total < 30?}
    D -- No --> P
    P --> Q
    Q --> R
    R --> B

    E[Esperar componente]
    F[Capturar imagen]
    G[Clasificar componente]
    H{Confirmar clasificación}
    I[Realizar Pick]
    J[Aproximación Pick]
    K{Confirmar Pick}
    L[Realizar Place]
    M[Confirmar Place]

    N{¿Place OK?}

    O[Actualizar inventario]
    

    P[Finalizar etapa]
    Q[Esperar Reset]
    R([Reiniciar proceso])

    A --> B
    B --> C
    C --> D

    D -- Sí --> E
    E --> F
    F --> G
    G --> H
    H -- Sí --> I
    H -- Descartar --> E
    I --> J
    J --> K
    K --  Sí--> L
    K -- Descartar --> E
    L --> M
    M --> N

    N -- Sí --> O
    O --> D
    

    N -- No --> D

    classDef subrutina fill:#C8E6C9,stroke:#2E7D32,stroke-width:2px;
    class H,K subrutina;
```

## Subrutina de clasificación
```mermaid
flowchart TD

A([Inicio confirmar clasificación])

B[Mostrar mensaje de espera de confirmación]

C{¿El componente es correcto?}

C -->|1: Sí| D
D --> N
N --> O


D[Avanzar banda transportadora<br/>1.4 s]

E[Retroceder banda<br/>1.5 s]

F[Esperar componente]

G[Capturar imagen]

H[Clasificar componente]

I[Descartar componente]

N[Reiniciar variable<br/>ConfirmarClasificación = 0]

O([Fin])

A --> B
B --> C
C -->|2: No| E
E --> F
F --> G
G --> H
H --> C
C -->|3: Descartar| I
```

## Subrutina de pick

```mermaid
flowchart TD

A([Inicio confirmar pick])

B[Mostrar mensaje de espera de confirmación]

C{¿Se realizó correctamente el Pick?}

D[PICK confirmado]

E[Reintentar PICK]


G[Descartar componente]


N[Reiniciar ConfirmarPick = 0]

O([Fin])

A --> B
B --> C

C -->|1: Sí| D
D --> N

C -->|2: No| E
E --> C
N --> O
C -->|3: Descartar| G
```

## Subrutina de interrupción

Además, se agrega una parada de emergencia por software. Esta rutina es agregada como una interrupción, lo que permitirá parar al manipulador en el momento que sea necesario, imponiéndose sobre el código que se esté ejecutando.

```mermaid
flowchart TD

A([Interrupción de emergencia])

B[Mostrar mensaje de parada<br/>de software]

C[Apagar actuadores y banda transportadora]

E[Esperar botón Inicio]

G[Mostrar mensaje de reanudación de proceso]

H([Retornar al proceso])

A --> B
B --> C
C --> E
E --> G
G --> H
```
Sin embargo, es importante mencionar que esta parada de emergencia por software no es ideal, ni comparable, con respecto a las paradas de emergencia físicas. En este caso, la interrupción funciona al terminar la línea de código que se está ejecutando, por lo tanto, su parada no es completamente instantánea, lo que podría suponer un riesgo para el entorno a nivel general o el personal que esté alrededor del manipulador. Además, tener que utilizar un botón adicional en la interfaz no es tan práctico y alcanzable como el botón de parada de emergencia físico, ya que supone un paso adicional e incluso que puede no llegar a ejecutarse por algún error o desviación al tocar el botón en la pantalla. Todas estas razones suponen una gran desventaja a la parada de emergencia por software frente a los botones físicos.

---

# 6. Código fuente comentado: RAPID / Python / SPEL+.
Código fuente de utilizado para el desarrollo de la práctica.
## Descripción de las funciones utilizadas.

# 7. Visión de máquina

Para poder realizar la clasificación de los cuatro objetos, hicimos uso de visión de máquina. Como se mencionó con anterioridad, se seleccionó hacer uso de YOLO para cumplir esta función. Para poder implementarlo, fue necesario descargar la librería de Ultralytics que contiene el modelo YOLO, haciendo uso del comando "pip install ultralytics". Tras esto, se procedió a tomar las fotos necesarias para entrenar el modelo de visión de máquina. Cabe señalar que se tomaron dos “sesiones” de fotos, ya que tras las primeras se realizaron algunos cambios, por lo que para mantener el modelo lo más fiel a lo que va a observar la cámara fue necesario volver a tomar fotos. Además, la primera "sesión" fue tomada con un celular, por lo que la calidad de la imagen es diferente a la de la webcam que se terminó usando.

<p align="center">
<img src="images/Roboflow.png" width="350">
<br>
<b>Figura. Datasets empleados para el entrenamiento. </b>
</p>

Luego de tomar las fotos, se procedió a etiquetarlas, para indicar a qué objeto corresponde cada foto, además de indicar la región de la imagen donde se encuentra, esto para evitar que tome parte del fondo para reconocer el objeto. Esta tarea se realizó en la plataforma Roboflow. El primer dataset contiene en total 87 imágenes, y el segundo contiene 174 imágenes. Luego se aplica data augmentation para darle más variedad al dataset. Este dataset se descarga para luego usarlo de manera local para el entrenamiento.

Tras realizar el entrenamiento, se aplicó el modelo de visión de máquina para identificar los cuatro objetos haciendo uso de una webcam conectada por USB a la PC. En este programa, inicialmente debemos iniciar la comunicación con el puerto serial COM, en este caso COM3; esto se hace con el fin de poderse comunicar con el Arduino Uno, tema que se tratará más adelante. Luego cargamos el modelo anteriormente entrenado e iniciamos la cámara. A lo largo del programa se emiten algunos mensajes para confirmar la comunicación entre los dispositivos.

---

## Descripción de las funciones utilizadas.

### Python/YOLO

En el programa train_det.py se maneja únicamente una función, la cual a su vez hace uso de un método para realizar el entrenamiento, el cual lo ofrece Ultralytics. Este método recibe como argumentos data, epochs, imgsz, batch, workers, amp, pretrained, cache, project, name. Este método pertenece al objeto model que se crea al inicio de la función, al cual se le asigna el modelo del cual va a partir para realizar el entrenamiento, también llamado finetuning. El primer entrenamiento realizado usó YOLOv26m como punto de partida. Además, este se realizó haciendo 30 "repeticiones" o epochs, redimensionando las imágenes a 640 píxeles, procesando ocho imágenes simultáneamente por paso de entrenamiento, aceptando los pesos del modelo preentrenado y activando el entrenamiento con precisión mixta automática, la cual permite reducir el uso de la memoria de la GPU, ya que se está usando CUDA. Luego de entrenarlo, se tomó el modelo entrenado y se reentrenó usando el segundo dataset. En los anexos se puede consultar el archivo con el que se entrenó.

El último entrenamiento realizado ofreció la siguiente matriz de confusión. En esta podemos observar que, salvo algunas excepciones en la prueba que realiza el modelo, la mayoría de imágenes fueron clasificadas correctamente.

<p align="center">
<img src="images/Matriz_final.png" width="350">
<br>
<b>Figura. Matriz de confusión final. </b>
</p>

Para usar la detección ya con la cámara, se utiliza el programa camara_deteccion.py; este no contiene funciones propias en su código, únicamente se ejecuta de manera secuencial. En este código se encuentra un bucle while que se encarga de estar constantemente leyendo el puerto serial, esperando el mensaje “IR”; en el momento que recibe este mensaje, procede a tomar una captura de la cámara usando el método cap.read(). Tras realizar la captura, se procede a realizar el análisis con el modelo previamente cargado desde el entrenamiento. Tras usar el modelo para reconocer algún objeto, se guarda la imagen de los resultados y luego se evalúa la condición de que si se detectase algún objeto en la captura. En caso de que no, entonces se emite un mensaje indicando que no se detectó ningún objeto. En el caso contrario, entonces, del mejor resultado se extrae la clase identificada y su confiabilidad. Dado que la numeración que realiza YOLO de las clases es diferente a la numeración que recibe el Arduino para distinguir a qué clase corresponde la clase, es necesario sumarle 1 al valor que obtiene YOLO. Luego de imprimir en consola el nombre de la clase, la confianza y el número de la clase, se envía por serial al Arduino este número. Para evitar que haya problemas en la comunicación y que no reciba nuevamente el mensaje IR, se debe configurar un ciclo while que espera a recibir el mensaje “Recibido” desde el Arduino. Cuando recibe este mensaje, muestra en pantalla una imagen con la clasificación realizada, junto con su confiabilidad y con la caja envolvente.

<div align="center">
  <table>
    <!-- Primera Fila -->
    <tr>
      <td align="center">
        <img src="images/nulo.png" width="300"><br>
        <b>(a)</b>
      </td>
      <td align="center">
        <img src="images/capacitor.png" width="300"><br>
        <b>(b)</b>
      </td>
    </tr>
    <!-- Segunda Fila -->
    <tr>
      <td align="center">
        <img src="images/conector.png" width="300"><br>
        <b>(c)r</b>
      </td>
      <td align="center">
        <img src="images/resistencia.png" width="300"><br>
        <b>(d)</b>
      </td>
    </tr>
    <!-- Tercera Fila (Imagen Centrada) -->
    <tr>
      <td align="center" colspan="2">
        <img src="images/ci.png" width="300"><br>
        <b>(e)</b>
      </td>
    </tr>
  </table>
  <br>
  <b>Figura . </b> Evidencias de detección: (a) Nulo, (b) Capacitor, (c) Conector, (d) Resistencia y (e) Circuito Integrado.
</div>


### Arduino

En Arduino se utilizaron las dos funciones principales, setup y loop, además de dos adicionales propias. La primera es la función detectarIR, la cual responde a una interrupción provocada por la entrada 2 nombrada con “sensorIR”. Esta función únicamente coloca en alto una bandera (“ifInf”) que luego se emplea en la función loop. La segunda función propia tiene el propósito de apagar las salidas del Arduino (8, 9, 10, 11); sin embargo, la lógica entre el Arduino y el controlador está negada, por lo que en la función se encienden todas estas salidas.

En la función loop, donde se encuentra el programa principal, se parte de revisar la bandera (“ifInf”) que indica si se le dio la orden de comunicarse con el computador para revisar la detección de objetos. Por lo tanto, si recibe esta señal, procede a enviar por serial el mensaje “IR”, que le indica a la PC que detecte y devuelva cuál es la clasificación que reconoce. Luego se queda esperando la respuesta, y al recibirla tiene un switch-case con el cual, dependiendo del mensaje que reciba, enciende (apaga por la lógica negada) una de las salidas, indicándole así al controlador del robot a cuál clase corresponde el objeto que está en la banda transportadora.

---

# 8. Plano de planta

---

# 9. Esquemático de conexiones

Para la realización del esquema, se tuvo en cuenta la conexión entre el Arduino y el controlador del manipulador, omitiendo así la conexión entre el Arduino y el computador y la cámara, ya que el Arduino únicamente se conecta con el cable USB para hacer uso del puerto serial, así como la cámara se conecta a otro puerto USB del computador.

<p align="center">
<img src="images/Esquemático.png" width="350">
<br>
<b>Figura. Esquemático del circuito utilizado en el proyecto. </b>
</p>

Para consutar el esquema con mas detelle reviser el anexo: [Esquemático](Anexos/Esquematico-v1.pdf)

---


# 10. Video de simulación

[Simulación](https://www.youtube.com/watch?v=sp3Va0xPWoQ)

---

# 11. Video de implementación física

[Implementación](https://www.youtube.com/watch?v=KdvNrrpuJQg)

---

# 12. Conclusiones

---

# Referencias
