#ifndef EMULADOR_6502_PIPELINE_6502_H
#define EMULADOR_6502_PIPELINE_6502_H

/**** PROTOTIPOS DE FUNCIONES DEL 6502 ****/
void init();
void reset_cpu();
void fetch();
void decode();
void execute();

#endif //EMULADOR_6502_PIPELINE_6502_H
