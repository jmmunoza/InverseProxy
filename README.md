# InverseProxy

## 1. Introducción

Este proyecto de balanceador de carga proxy ha sido desarrollado en el poco amado pero necesario C. El proyecto ha sido desarrollado únicamente por Juan Manuel Muñoz Arias (202010100010). En los siguentes párrafos explicaré el desarrollo de este proyecto.

## 2. Desarrollo

### a. API Sockets.

El primer paso a fue investigar acerca de los sockets en C. Realizada la investigación, descubrí la existencia de dos librerías distintas acorde al sistema operativo, teniendo a "winsock2.h" por el lado de Windows, el sistema operativo utilizado para el desarrollo del proyecto.

### b. Reciviendo y enviando mensajes

El siguiente paso fue comenzar a jugar con los sockets en un ambiente local, realizando tanto peticiones como respuestas. Este fue uno de los pasos más largos. Ya que al comienzo poseía un conocimiento muy bajo en torno a C, algo que mejoré con la práctica.

### c. LOG

Para el desarrollo de los LOGs fue sencillo. Solamente registraba las peticiones y las respuestas, imprimiéndolas en pantalla y guardándolas en un archivo .txt automáticamente mediante el método de escritura "a" (append).

### d. Caché

El caché fue el componente que más problemas me presentó, debido, otra vez, a mi bajo conocimiento en C. Para realizar el caché, tomé las peticiones y sus respectivas respuestas, creando archivos en base al nombre de la paticion. 

Por ejemplo, si mi petición era "GET / HTTP/ 1.1", creaba un archivo con el nombre "GETHTTP11" y dentro de este escribía el response y lo guardaba dentro del disco en una carpeta llamada "cache". Para obtener los datos del caché en caso de error, simplemente es convertir la petición HTTP en el nombre del archivo para retornarlo al usuario, funcionando como una especia de tabla. La TTL que implementé fue de 20 segundos. Los problemas que se presentaron fueron más que todo por temas de manejo de la memoria, pero la prueba y el error fueron suficientes para solucionarlo.

### d. Archivo de configuración

Este componente también es sencillo, para configurar las IPs y puertos del proxy hay que hacer uso de un archivo de configuración con la siguiente sintaxis:

127.0.0.1   // Direccion del proxy (Siempre sera la primera direccion en la lista)
8080        // Puerto del proxy
127.0.0.1   // Direccion del server 1
8000        // Puerto del server 1
145.37.12.1 // Direccion del server 2
8080        // Puerto del server 2
124.1.21.1  // Direccion del server 3
8080        // Puerto del server 3
66.13.0.1   // Direccion del server n
8080        // Puerto del server n
            // Siempre dejar un espacio al final
            
Como se puede ver, el primer puerto y dirección son los correspondientes al servidor proxy, el resto de valores corresponden a los servidores en donde está alojado el servicio Web. Siempre hay que dejar un espacio en blanco al final para que el archivo sea leido correctamente por el reader.

### e. Round Robin

Para realizar esta rotación, se toman las IPs y puertos en una lista y se comienzan a rotar con un contador a medida que entran peticiones. Con cada rotacion, se crea un socket hacie el Web server correspondiente, se recibe, se envía al cliente y se cierra el socket y continua el proceso.

### f. Despliegue

Con el proxy listo, es tiempo para crear un servicio Web. Mediante el uso de Django y python creé una página web sencilla para probar que todo funcionara, la instalé en cada una de las máquinas virtuales de AWS, configuré sus IPs, tanto privadas como públicas, y el servicio estaba listo para correr. Por el lado del proxy, como se trataba de una app hecha en Windows, requerí hacer el proces en una máquina virtual de Windows, pero el proceso es el mismo. Se configuran los puertos y direcciones y queda todo desplegado.

## 3. Conclusiones

- El uso de C puede variar drásticamente entre sistemas operativos.
- Siempre que quieras conectar tu Web App para recibir peticiones, deberá funcionar en la IP privada que te brinde AWS para que se pueda conectar con la IP pública. 
- Hay algunos problemas en C con ciertos responses ya que se pierden caracteres, he llegado a la conclusión de que se trata de algo referente al codificado UTF-8, pero no fuí capaz de solucionarlo.
- Con esta práctica fortalecí todo lo visto en clase, como el manejo de IPs privadas y públicas, la programación en red, y el despligue de servicios Web. Algo que me emociona mucho.  
- El balanceador de carga es una herramienta poderosa para evitar el colapso de las aplicaciones, algo que no conocía y que he profundizado con esta práctica.
- El proceso LOG es sumamente importante ya que me permitió identificar errores de una manera más facil.

## 4. Referencias

- https://learn.microsoft.com/en-us/windows/win32/api/winsock2/
