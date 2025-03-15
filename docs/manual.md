# Manual de Usuario: Robot Seguidor de Líneas

## Introducción

Este manual proporciona instrucciones para operar correctamente el robot seguidor de líneas con detección de obstáculos. El robot ha sido diseñado para fines educativos y de demostración de conceptos de robótica.

## Preparación

1. **Verificación de energía**:
   - Instale las baterías en el soporte.
   - Compruebe que el indicador de energía se encienda.

2. **Superficie de operación**:
   - Prepare una superficie plana con una línea negra de aproximadamente 1.5-2cm de ancho.
   - Evite superficies reflectantes que puedan confundir al sensor.

3. **Condiciones de luz**:
   - El robot funciona mejor en condiciones de luz moderada.
   - Evite luz directa sobre el sensor de línea.

## Operación

1. **Encendido**:
   - Coloque el robot sobre la línea negra.
   - Active el interruptor de encendido.
   - Espere a que el robot complete su secuencia de inicialización (parpadeo de LEDs).

2. **Indicadores**:
   - **LED Verde encendido**: El robot está siguiendo la línea correctamente.
   - **LED Rojo encendido**: El robot ha detectado un obstáculo y está evaluando opciones.

3. **Comportamientos**:
   - **Seguimiento normal**: El robot avanza siguiendo la línea negra.
   - **Evasión de obstáculos**: Al detectar un obstáculo, el robot detendrá su avance, escaneará el entorno y elegirá la mejor ruta alternativa.
   - **Búsqueda de línea**: Si el robot pierde la línea, realizará movimientos de búsqueda para encontrarla nuevamente.

4. **Apagado**:
   - Desactive el interruptor de encendido cuando termine de usar el robot.
   - Retire las baterías si no utilizará el robot por un período prolongado.

## Solución de Problemas

| Problema | Posible Causa | Solución |
|----------|---------------|----------|
| El robot no enciende | Baterías agotadas | Reemplace las baterías |
| No sigue la línea | Sensor desalineado | Calibre la posición del sensor |
| Giros excesivos | Velocidad muy alta | Ajuste BASE_SPEED en el código |
| No detecta obstáculos | Sensor ultrasónico desconectado | Verifique las conexiones |
| Movimientos erráticos | Interferencia en la superficie | Limpie la superficie y la línea |

## Mantenimiento

- Limpie periódicamente los sensores con aire comprimido.
- Verifique y ajuste las ruedas si es necesario.
- Inspeccione las conexiones de cables sueltas.
- Mantenga la batería cargada para un rendimiento óptimo.

## Personalización

El robot puede ser personalizado modificando el código fuente. Consulte el archivo `informe_tecnico.md` para más detalles sobre la estructura del programa y posibles modificaciones.