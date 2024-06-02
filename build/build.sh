arm-none-eabi-gcc -x assembler-with-cpp -c -O0 -mcpu=cortex-m0 -mthumb -Wall -fmessage-length=0 ../start_up/start_up.s -o start_up.o
arm-none-eabi-gcc -x assembler-with-cpp -c -O0 -mcpu=cortex-m0 -mthumb -Wall -fmessage-length=0 ../start_up/vector_table.s -o vector_table.o

arm-none-eabi-gcc -c -fmessage-length=0 -Wall -g -mthumb -mcpu=cortex-m0 ../stm_32_ports.c -o stm_32_ports.o

arm-none-eabi-gcc start_up.o vector_table.o stm_32_ports.o -mcpu=cortex-m0 -mthumb -Wall --specs=nosys.specs -nostdlib -lgcc -T../start_up/stm_linker.ld -o stm_32_ports.elf

arm-none-eabi-objcopy -S -O binary stm_32_ports.elf stm_32_ports.bin
arm-none-eabi-objcopy -S -O ihex stm_32_ports.elf stm_32_ports.hex
arm-none-eabi-size stm_32_ports.elf
