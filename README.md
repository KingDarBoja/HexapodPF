# Autonomous Hexapod for navigation in unknown environments using Fuzzy Logic

<h3>English</h3>
Development of autonomous hexagonal hexapod robot for navigation in unknown environments using fuzzy logic ruleset to handle obstacle avoidance through ultrasonic sensor measure and turning / linear speed using gyroscope / acelerometer.

<h3>Español</h3>
Desarrollo de un robot hexapodo hexagonal autonomo para navegación en entornos desconocidos utilizando set de reglas de Lógica Difusa para esquivar obstaculos a tráves de la medida de sensores ultrasonicos y de la velocidad de giro / linear utilizando un giroscopio / acelerómetro.

<h4>Conexión inalambrica con modulos Xbee</h4>
La documentación completa se puede encontrar en su sitio oficial o para mayor comodidad, la página [Aprendiendo Arduino][4] le informa todo lo que necesita saber sobre los modulos completamente en español.

La configuración inicial de los modulox Xbee se realiza con el software **_XBee Configuration and Test Utility (XCTU) _**, siguiendo las instrucciones en el enlace previamente mencionado o en su defecto, para mayor rapidez este [enlace][5].

<h4>Desarrollo de la Lógica Difusa</h4>
Para la toma de decisiones por parte del sistema inteligente, se utiliza la herramienta _Fuzzy Logic_ disponible en el software de **_MatLab_** para estudiantes. Con el fin de no profundizar en su funcionamiento, recomendamos ver los [tutoriales][3] disponibles en el sitio web de MatLab.

<h4>Configuración del MPU-6050</h4>
Para la medición del angulo de inclinación, se utiliza una _unidad de medición inercial_ o **_IMU_** la cual consiste de un giroscopio y acelerómetro de tres ejes, es decir, 6 grados de libertad (**6DOF**) siendo en este caso un [MPU-6050 Modelo GY521][1].

Los codigos para obtener los valores en bruto y hacer la respectiva normalización y correciones de offset y el filtro complementario para el error se basaron de este [enlace][2].

En nuestro proyecto, las configuraciones del MPU-6050 y sus funciones se encuentran en el archivo ```src/gyroscope.cpp```.


[1]: https://playground.arduino.cc/Main/MPU-6050
[2]: https://robologs.net/2014/10/15/tutorial-de-arduino-y-mpu-6050/
[3]: https://www.mathworks.com/videos/getting-started-with-fuzzy-logic-toolbox-part-1-68764.html
[4]: https://aprendiendoarduino.wordpress.com/tag/xbee/
[5]: https://hangar.org/webnou/wp-content/uploads/2012/01/arduino-xbee-primeros-pasos.pdf
