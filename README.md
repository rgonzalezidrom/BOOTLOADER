# Bootloader V1.0
Bootloader V1.0 es la versión actualizada (23/11/2029), es un Bootloadre diseñado a medida para el microcontrolador PIC18F4620, que fuencion en conjunto con la aplicacion de escritorio de Windows IdromPicBootloader.exe

## Contenido
Este proyecto tiene dos branch: **main** y **develop**, *main* contiene la version estable y actualizada de todo los codigos del firmware para el bootloader y el programa para cargar los firware via puerto serial  



## Codigos Fuente Bootloadres 
 En la carpeta firmware, se encuentre el codigo fuente fuente esta desarrollado en AMSEMBLER (*Bootloader_IDROM_codigo_fuente.x*), requiere de las librerias spbrgselect.inc y bankswitch.inc para que el condigo fuente del booloader (.ams), El entorono de desarrollo y compilacion es IDE mplabx V5.35 y el compilador xc8 V2.31, en la misma carpeta se encuentre el codigo fuente del Led y el Buzzer en modo Blink (*Leb_Buzzer_Blink-Codigo_fuente.X*), codig0 desarrollo en *C* con el IDE mplabx V5.35 y el compilador xc8 V2.31

 
## Firmware de producción
El firmware de producion del bootloader es un archivo binario con extencion hexadecimal (.hex), con el nombre bootloader_atenea_Idrom_V10.hex, es una version que tiene integrada los dos codigos fuentes anteriores, este se encuenta en la  la carpeta /firmware/Bootloader_IDROM_produccion/

## Como cargar el bootloader

Para cargar el bootloader al microcontrolador pic18f4620 el fabricante de las PCB o responsable técnico requiere de un programdor de pic, de la microchip, podemos usar el PITkit4 o el ICD 3. en el siguiente documento se muestra los pasos para que el personal técnico lo pueda realizar desde la placa *Atenea Mainboard*

![](/contenido/IT-16-Ed-1-Carga-de-bootloader-en-Mainboard.pdf)

## Programa de carga par hex 
 En la carpeta BootLoader IdromLT, se encuentre el programa para cargar los progrmas Hexadecimales (.hex), este programa se usa luedo de tener cargado el bootloader al pic


### Notas (LEER) :

## 
Cuando las placas llegan por primera vez, con el bootloadre, tiene una secuendia en el encendido del LDB RGB y el buzzer comienza a emitir sonidos cada 1 segundo
Alguna duda o verificacion puedes enviar un correo a r.gonzalez@idromgroup.com