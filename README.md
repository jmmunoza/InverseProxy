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
            
Como se puede ver, el primer puerto y dirección son los correspondientes al servidor proxy, el resto de valores corresponden a los servidores en donde está alojado el servicio Web.

## 3. Conclusiones

## 4. Referencias
