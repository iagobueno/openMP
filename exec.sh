#!/bin/bash

OUTPUT=results.txt

if [ -f ${OUTPUT}  ]; then
	rm ${OUTPUT}
fi

echo "Sequencial" >> ${OUTPUT} 
echo "" >> ${OUTPUT}

{ time ./seq < tsp.in; } >> $OUTPUT 2>&1 

echo "" >> ${OUTPUT}
echo "Paralelo" >> ${OUTPUT}
echo "" >> ${OUTPUT}

{ time ./par < tsp.in; } >> $OUTPUT 2>&1
