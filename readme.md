## Ejercicio 2

Programas End Device y Coordinador de forma que se adquiera información sobre la temperatura corporal y se envíe de forma 
inalámbrica al Coordinador. La información biomédica puede ser enviada en forma de string o cadena de caracteres

## Ejercicio 3

Se desea monitorizar simultáneamente temperatura y pulsioximetría con un periodo de adquisición de 3 segundos.
Crea los programas necesarios para el End Device y Coordinador. La información biomédica puede ser enviada en forma de
string o cadena de caracteres, y debe incluir una referencia temporal relativa para conocer el instante de adquisición

## Ejercicio 4

En cualquier transmisión de información es habitual utilizar algún protocolo de
comunicación, bien propietario, bien estandarizado. En este caso, se desea enviar
la información biomédica correspondiente al Ejercicio 3 en mensajes con la
siguiente estructura:
      Header                    Payload
Protocol  DataType  SenderID  TimeReference   Measurement   
1 Byte    1 Byte    3 Bytes   9Bytes          N bytes

Crea los programas para el End Device y Coordinador que realice la transmisión y
recepción de la información de acuerdo al protocolo indicado. El Coordinador
debe ser capaz de separar e identificar el valor de cada campo del
mensaje y mostrar en pantalla la información relevante.

## Ejercicio 5

Las especificaciones del cliente para la plataforma son abiertas, es decir, el
alumnado deberá tomar decisiones basadas en sus capacidades y en la
experiencia adquirida en este Módulo 2. Dichas especificaciones son:
1) La temperatura corporal se deberá enviar tres veces al día cada ocho horas,
por ejemplo, 8:00-16:00-23:59 (aproximadamente)
2) El peso, en bascula pesa personas habitual, se realizará a las 9:00 horas
aproximadamente para pacientes con insuficiencia cardiaca
3) El ritmo cardiaco y la SpO2 (saturación de oxígeno en sangre o pulsioximetría)
deberá ser una monitorización continua a lo largo del día. La periodicidad
puede ser entre 1 y 2 segundos
4) Resulta de interés disponer de algún tipo de alarma simple principalmente en
la temperatura, ritmo cardiaco y saturación de oxígeno. Límite máximo para la
temperatura: 38 o C, límite mínimo para la SpO2: 95%, y limites máximo y
mínimo para el ritmo cardiaco: 35-110 BPM.
