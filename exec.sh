#!/bin/bash

OUTPUT=results.txt

if [ -f ${OUTPUT}  ]; then
	rm ${OUTPUT}
fi

echo "Sequencial" >> ${OUTPUT} 

{ time ./seq < tsp.in; } >> $OUTPUT 2>&1 

echo "Paralelo" >> ${OUTPUT}

{ time ./par < tsp.in; } >> $OUTPUT 2>&1
