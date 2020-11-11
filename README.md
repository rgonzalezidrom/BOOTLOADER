# Bootloader V1.0
Bootloader V1.0 es la versión actualizada el 11/11/2029, es un Bootloadre diseñado a medida para el microcontrolador PIC18F4620

## Contenido
Este proyecto tiene solo una branch: **main** contiene de forma estable y actualizada los codigos del firmware y el programa para cargar los nuevos codigos via puerto serial  



## Codigos Fuente Bootloadres 
 En la carpeta firmware, se encuentre el codigo fuente fuente esta desarrollado en AMSEMBLER BootloaderOSC_HS_IDROM.asm y requiere de las librerias spbrgselect.inc y bankswitch.inc, para el IDE mplabx V5.35 y el compilador xc8

 ## Programa de carga par hex 
 En la carpeta BootLoader IdromLT, se encuentre el programa para cargar los progrmas Hexadecimales (.hex), este programa se usa luedo de tener cargado el bootloader al pic


## Firmware de producción
El firmware de producion del bootloadre es un archivo binario con extencion exadecimal (.hex), con el nombre bootloadre_IDROM.X.production.hex, este se encuenta en la siguiente ruta de la carpeta /firmware/bootloadre_IDROM.X/dist/default/production/

## Como cargar el bootloader

Para cargar el bootloader re requiere de un programdor de pic, de la micro chip, podemos usar el PITkit2 o el ICD 3


### Notas:

Cuando las placas llegan por primera vez, con el bootloadre, tiene una secuendia en el encendido del LDB RGB y el buzzer comienza a emitir sonidos cada 1 segundo
Alguna duda o verificacion puedes enviar un correo a r.gonzalez@idromgroup.com