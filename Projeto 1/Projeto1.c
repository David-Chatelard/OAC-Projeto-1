/*
Trabalho 01 da displina de Organização e Arquitetura de Computadores
Turma: C
Aluno: David Fanchic Chatelard
Matrícula: 180138863
IDE utilizada: Visual Studio Code
Sistema operacional: Windows 10
Compilador: GCC
*/

#include <stdio.h>
#include <stdint.h>

#define MEM_SIZE 4096
int32_t mem[MEM_SIZE];

int32_t lw(uint32_t address, int32_t kte){
    int32_t word_address = address + kte;
    if (word_address % 4 != 0 || word_address / 4 > (MEM_SIZE - 1) || word_address < 0){
        printf("Endereco invalido.\n");
        return 0;
    }
    else{
        return mem[word_address / 4];
    }
}

int32_t lb(uint32_t address, int32_t kte){
    int32_t palavra;
    int32_t byte_palavra;
    uint32_t mascara;

    if (address % 4 != 0 || address / 4 > (MEM_SIZE - 1) || address / 4 < 0){
        printf("Endereco invalido.\n");
        return 0;
    }
    if (kte > 3 || kte < 0){
        printf("Byte invalido.\n");
        return 0;
    }

    palavra = lw(address, 0);
    mascara = 255 << (8 * kte); //o mascaramento vai ser feito com and, então essa instrução faz com que o byte de 1's da máscara fique na posição correta para pegar o byte certo da palavra
    byte_palavra = palavra & mascara;   //byte_palavra vai receber só o byte desejado da palavra
    byte_palavra = byte_palavra >> (8 * kte);   //o byte vai ficar alinhado à direita, caso o msb seja 0 não é necessário estender o sinal, pois o restante da palavra já irá possuir 0's

    if ((int8_t)byte_palavra < 0){  //caso o msb seja 1
        mascara = 0xFFFFFF00;   //mascara para botar 1's no resto do byte_palavra para estender o sinal
        byte_palavra = byte_palavra | mascara;  //completa o resto da palavra com 1's
    }

    return byte_palavra;
}

int32_t lbu(uint32_t address, int32_t kte){
    int32_t palavra;
    int32_t byte_palavra;
    uint32_t mascara;

    if (address % 4 != 0 || address / 4 > (MEM_SIZE - 1) || address / 4 < 0){
        printf("Endereco invalido.\n");
        return 0;
    }
    if (kte > 3 || kte < 0){
        printf("Byte invalido.\n");
        return 0;
    }

    palavra = lw(address, 0);
    mascara = 255 << (8 * kte); //o mascaramento vai ser feito com and, então essa instrução faz com que o byte de 1's da máscara fique na posição correta para pegar o byte certo da palavra
    byte_palavra = palavra & mascara;   //byte_palavra vai receber só o byte desejado da palavra
    byte_palavra = byte_palavra >> (8 * kte);   //o byte vai ficar alinhado à direita
    mascara = 255;  //mascara com 8 1's no começo e 0 no resto
    byte_palavra = byte_palavra & mascara;  //faz todos os bits, exceto os 8 primeiros, serem 0

    return byte_palavra;
}

void sw(uint32_t address, int32_t kte, int32_t dado){
    int32_t word_address = address + kte;

    if (word_address % 4 != 0 || word_address / 4 > (MEM_SIZE - 1) || word_address < 0){
        printf("Endereco invalido.\n");
        return;
    }

    mem[word_address / 4] = dado;
}

void sb(uint32_t address, int32_t kte, int8_t dado){
    int32_t palavra;
    int32_t dado2;  //variavel que tera o valor de dado, só que em 32 bits
    int32_t mascara_aux = 0x000000FF;
    uint32_t mascara = 0xFFFFFFFF;

    if (address % 4 != 0 || address / 4 > (MEM_SIZE - 1) || address / 4 < 0){
        printf("Endereco invalido.\n");
        return;
    }
    if (kte > 3 || kte < 0){
        printf("Byte invalido.\n");
        return;
    }

    palavra = lw(address, 0);
    dado2 = 0x000000FF; //garantindo que dado2 terá 0 no restante da palavra
    dado2 = dado2 & dado;   //garantindo que dado2 terá o dado em seu byte menos significativo
    mascara_aux = mascara_aux << (8 * kte); //deixa o byte FF no local necessário
    mascara -= mascara_aux; //deixa a mascara com um byte 00 no local necessário, e o restante dos bytes com FF
    dado2 = dado2 << (8 * kte); //alinha o byte do dado para onde ele tem que estar na palavra
    palavra = palavra & mascara;    //vai deixar a palavra com 00 no local em que se deseja inserir o dado, e o restante da palavra permanece a mesma
    palavra = palavra | dado2;  //insere o dado na palavra, no local correto

    mem[address / 4] = palavra;
}

int main(){
    int32_t a0, a1, a2, a3, b0, b1 ,b2 ,b3, c0, c1 ,c2;
    
    //Testes do professor
    printf("Testes do professor:\n\n");
    sb(0, 0, 0x04); //a
    sb(0, 1, 0x03);
    sb(0, 2, 0x02);
    sb(0, 3, 0x01);

    sb(4, 0, 0xFF); //b
    sb(4, 1, 0xFE);
    sb(4, 2, 0xFD);
    sb(4, 3, 0xFC);

    sw(12, 0, 0xFF);    //c

    sw(16, 0, 0xFFFF);  //d

    sw(20, 0, 0xFFFFFFFF);  //e

    sw(24, 0, 0x80000000);  //f

    for (int i = 0; i < 7; i++){
        printf("mem[%d] = %08x\n", i, mem[i]);
    }

    printf("\n\n");

    //Teste lb
    a0 = lb(4, 0);
    a1 = lb(4, 1);
    a2 = lb(4, 2);
    a3 = lb(4, 3);

    //Teste lbu
    b0 = lbu(4, 0);
    b1 = lbu(4, 1);
    b2 = lbu(4, 2);
    b3 = lbu(4, 3);

    c0 = lw(12, 0);
    c1 = lw(16, 0);
    c2 = lw(20, 0);

    printf("Teste lb\n");
    printf("a0 = %08x\n", a0);
    printf("a1 = %08x\n", a1);
    printf("a2 = %08x\n", a2);
    printf("a3 = %08x\n", a3);

    printf("\n\n");

    printf("Teste lbu\n");
    printf("b0 = %08x\n", b0);
    printf("b1 = %08x\n", b1);
    printf("b2 = %08x\n", b2);
    printf("b3 = %08x\n", b3);

    printf("\n\n");

    printf("c0 = %08x\n", c0);
    printf("c1 = %08x\n", c1);
    printf("c2 = %08x\n", c2);

    printf("\n\n");

    printf("Testes do aluno:\n\n");

    printf("Teste de validacao de entradas\n\n");

    printf("Teste de lw\n");
    lw(11, 0);
    lw(0, 13);
    lw(-12, 0);
    lw(0, -16);
    lw(24000, 0);
    lw(0, 24000);
    printf("\n");

    printf("Teste de lb\n");
    lb(11, 0);
    lb(0, 5);
    lb(-12, 0);
    lb(0, -16);
    lb(24000, 0);
    printf("\n");

    printf("Teste de lbu\n");
    lbu(11, 0);
    lbu(0, 5);
    lbu(-12, 0);
    lbu(0, -16);
    lbu(24000, 0);
    printf("\n");

    printf("Teste de sw\n"); 
	sw(15,0,0x01);
	sw(0,15,0x01);
    sw(15,2,0x01);
	sw(-16,0,0x01);
	sw(8,-24,0x01);
	sw(24000,0,0x01);
	sw(0,24000,0x01);
	printf("\n");

    printf("Teste de sb\n"); 
	sb(15,0,0x01);
	sb(0,5,0x01);
	sb(-16,0,0x01);
	sb(8,-2,0x01);
	sb(24000,0,0x01);
	printf("\n\n");

    printf("Teste do funcionamento:\n\n");

    printf("Teste do lw\n");

    sw(8, 0, 0xFFFFFFFF);
    printf("lw(8, 0): %08x\n", lw(8, 0));
    printf("lw(4, 4): %08x\n", lw(4, 4));
    printf("lw(12, -4): %08x\n", lw(12, -4));
    printf("\n\n");

    printf("Teste do lb e lbu\n");

    sw(20, 0, 0xF19C046A);

    for (int i = 0; i < 4; i++){
        printf("Byte com lb: %08x\n", lb(20, i));
        printf("Byte com lbu: %08x\n", lbu(20, i));
    }

    printf("\n\n");

    printf("Teste do sw\n");

    sw(40, 0, 0x4A7BD35F);
	printf("sw com addres = 40. kte = 0. dado = 0x4A7BD35F: mem[10] = %08x\n", mem[10]);
	sw(32, 8, 0x31954FCA);
	printf("sw com addres = 32. kte = 8. dado = 0x31954FCA: mem[10] = %08x\n", mem[10]);
	sw(48, -8, 0xB5FC18E3);
	printf("sw com addres = 48. kte = -8. dado = 0xB5FC18E3: mem[10] = %08x\n", mem[10]);

    printf("\n\n");

    printf("Teste do sb\n");

    for (int i = 0; i < 4; i++){
        printf("Palavra antes: %08x\n", mem[10]);
        sb(40, i, 0xFF);
        printf("Palavra depois: %08x\n", mem[10]);
        printf("\n");
    }

    return 0;
}
