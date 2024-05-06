## Huella Full

Este documento contiene detalles del mantenimiento de Huella Full

## Mecánica de des ensamble

1. Para empezar el des ensamblaje, es no necesario quitar los circuitos primero, pero es buena oportunidad para formalizar las conexiones.

    - La alimentación se conecta de forma directa a la placa del micro controlador, se agrega una terminal de tornillos.
    - Los calbes de datos y alimentación de los drivers y el multiplexor PWM para el control de motores están soldados, por lo que se deben extraer juntas las placas del micro controlador, PWM y drivers de motores DC
    - Los conectores a PWM y drivers están conectados por jumpers y terminales de tornillos, por lo que para desconectar esta sección, se requiere hacer un listado de conexiones.
    - Se recomienda acompañar el listado de conexiones con el desarrollo de un diagrama esquemático de las conexiones
    - Una vez identificadas, listadas y desconectadas todas las terminales de motores, se deben desatornillar las carcasas de los circuitos. Se desarrollarán carcasas nuevas para los circuitos.
2. Para poder acceder a los tornillos de sujección de las bases de motores, es necesario retirar la estructura metálica. Para ello se requiere seguir los siguientes pasos.

    - Retirar la membrana. Se aprovechará para limpiar todos los componentes de residuos de lubricante.
    - Desatornillar todas las esferas acopladas a ejes
    - Desatornillar la estructura metálica

3. En este punto es posible desatornillar cualquier pieza de la base de acrilico


## Tasks

- [ ] Agregar terminal de tornillo para la alimentación a la placa del micro controlador
- [ ] Listado de conexiones
- [ ] Circuito esquemático
- [ ] Diseñar nueva carcasa para los circuitos, estas deberán asegurar las placas con tornillos