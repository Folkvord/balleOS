@ECHO OFF

SET bootloader_dir=%CD%\bootloader\
SET asm_lib_dir=%bootloader_dir%\lib
SET kernel_dir=%CD%\kernel\
SET bin_dir=%CD%\bin\
SET obj_dir=%CD%\obj\
SET driver_dir=%CD%\drivers\
SET cpu_dir=%CD%\cpu\
SET util_dir=%CD%\util\
SET compiler=C:\Users\Probo\Documents\ProgrammeringsFiler\OS\cross_compilers\ghost-i686-elf-tools\bin\i686-elf-gcc.exe
SET linker=C:\Users\Probo\Documents\ProgrammeringsFiler\OS\cross_compilers\ghost-i686-elf-tools\bin\i686-elf-ld.exe
SET kernel_location=0x1000

:: LA TIL NYE FORLØKKER FOR CPU- OG UTIL-DIR
:: TODO: 
:: DRA OBJ OG BIN UT FRA KERNEL INN TIL HOVED DIR
:: PRØV Å FIKS LINKEREN

:: Kompiler alle "kernel_dir" filer
FOR /r %kernel_dir% %%f in (*.c) DO %compiler% -ffreestanding -m32 -g -c %%f -o %obj_dir%%%~nf.o
:: Kompiler alle "driver_dir" filer
FOR /r %driver_dir% %%f in (*.c) DO %compiler% -ffreestanding -m32 -g -c %%f -o %obj_dir%%%~nf.o
:: Kompiler alle "cpu_dir" filer
FOR /r %cpu_dir% %%f in (*.c) DO %compiler% -ffreestanding -m32 -g -c %%f -o %obj_dir%%%~nf.o
:: Kompiler alle "util_dir" filer
FOR /r %util_dir% %%f in (*.c) DO %compiler% -ffreestanding -m32 -g -c %%f -o %obj_dir%%%~nf.o

:: Kompiler til objektfiler
::%compiler% -ffreestanding -m32 -g -c %kernel_dir%kernel.c -o %obj_dir%kernel.o
::if NOT %errorlevel% == 0 (
::    echo C-KOMPILATOR FEIL (Kernel)
::)
::%compiler% -ffreestanding -m32 -g -c %driver_dir%ports.c -o %obj_dir%ports.o
::if NOT %errorlevel% == 0 (
::    echo C-KOMPILATOR FEIL (Ports-driver)
::)
::%compiler% -ffreestanding -m32 -g -c %driver_dir%screen.c -o %obj_dir%screen.o
::if NOT %errorlevel% == 0 (
::    echo C-KOMPILATOR FEIL (Screen-driver)
::)
::%compiler% -ffreestanding -m32 -g -c %kernel_dir%util.c -o %obj_dir%util.o
::if NOT %errorlevel% == 0 (
::    echo C-KOMPILATOR FEIL (Kernel-util)
::)
nasm %kernel_dir%kernel.asm -f elf -o %obj_dir%kernel_asm.o
if NOT %errorlevel% == 0 (
    echo ASM-KOMPILATOR FEIL (kernel_asm)
)
nasm %cpu_dir%interupts.asm -f elf -o %obj_dir%interupts_asm.o
if NOT %errorlevel% == 0 (
    echo ASM-KOMPILATOR FEIL (interupt_asm)
)

:: Link objektfilene sammen (Noe fandango når man bruker regex??? tror kernel location fuckes med)
%linker% -o %bin_dir%full_kernel.bin -Ttext %kernel_location% %obj_dir%kernel_asm.o %obj_dir%kernel.o %obj_dir%ports.o %obj_dir%screen.o %obj_dir%util.o %obj_dir%idt.o %obj_dir%isr.o %obj_dir%interupts_asm.o --oformat binary
::%linker% -o %bin_dir%full_kernel.bin -Ttext %kernel_location% %obj_dir%*.o --oformat binary >NUL 2>&1
if NOT %errorlevel% == 0 (
    echo LINKER FEIL: %errorlevel%
)

:: Sammenslå bootloaderen med kernelfilen
nasm -fbin %bootloader_dir%boot.asm -o %bootloader_dir%boot.bin -I%asm_lib_dir% >NUL 2>&1
if NOT %errorlevel% == 0 (
    echo ASM-KOMPILATOR FEIL (boot_loader)
)
copy /b %bootloader_dir%boot.bin + %bin_dir%full_kernel.bin %bin_dir%temp.bin >NUL 2>&1
if NOT %errorlevel% == 0 (
    echo CAT FEIL (1)
)

:: Lag et minnebuffer for emulatorsikkerhet
nasm -fbin %kernel_dir%membuffer.asm -o %bin_dir%membuffer.bin >NUL 2>&1
if NOT %errorlevel% == 0 (
    echo ASM-KOMPILATOR FEIL (membuffer)
)
copy /b %bin_dir%temp.bin + %bin_dir%membuffer.bin %CD%\balleOS.bin >NUL 2>&1
if NOT %errorlevel% == 0 (
    echo CAT FEIL (2)
)

:: Kjør dat shit
qemu-system-x86_64.exe %CD%\balleOS.bin