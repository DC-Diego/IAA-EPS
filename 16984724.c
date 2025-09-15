#include<stdio.h>
#include<stdlib.h>

int stringToInt(char * num){
  int i = 0;
  int newNum = 0;
  while(num[i]!='\0'){
    newNum = newNum*10+(num[i]-'0');
    i++;
  }
  return newNum;
}

int * montarMatriz(char * nomeArquivo, int m, int n){
  FILE * arquivo = fopen(nomeArquivo, "r");
  if(arquivo == NULL){
    printf("Erro ao abrir arquivo, o arquivo não existe");
    return NULL;
  }
  int * matriz = (int*) malloc(sizeof(int)*m*n);
  matriz[0] = 0;
  int i = 0;
  int c;
  while((c=fgetc(arquivo))!=EOF){
    if((char) c == ' ' || (char) c == '\n'){
      i++;
      matriz[i] = 0;
    }else{
      matriz[i] = matriz[i]*10+ (c-'0');
    }
  }
 
  fclose(arquivo);
  return matriz;
}

int floodFill(int * matriz,int linhas, int colunas, int x, int y){
  int profundidade;
  int posicao = x+y*colunas; 

  if(x >=0 && y>=0 && x < colunas&& y<linhas && matriz[posicao] != 0 ){
    profundidade = matriz[posicao];
    matriz[posicao] = 0;
  }else{
    return 0;
  }
  printf("FLOOD %d - pos: %d \n", profundidade, posicao);

  profundidade += floodFill(matriz, linhas, colunas, x+1,y); // x+1
  profundidade += floodFill(matriz, linhas, colunas, x-1,y); // x-1
  profundidade += floodFill(matriz, linhas, colunas, x,y+1); // y+1
  profundidade += floodFill(matriz, linhas, colunas, x,y-1); // y-1

  return profundidade;
}


int buscarIlhas(int * matriz, int linhas, int colunas){

  printf("\nRODEI\n");
  int qtdIlhas = 0;
  int* ilhasProfundidade = (int*) malloc(sizeof(int)*linhas*colunas/2+1); 
  
  for(int i = 0; i < linhas*colunas;i++){
    if(matriz[i]!=0){
      // int aux = 
      printf("ILHA %d\n", qtdIlhas);
      int aux = floodFill(matriz, linhas, colunas, i%colunas,i/colunas); 
      ilhasProfundidade[qtdIlhas++] = aux <6?1:aux/6;
      
    }
  }

  printf("%d qtdIlhas\n", qtdIlhas);
  for(int e = 0; e < qtdIlhas;e++)
    printf("%d ", ilhasProfundidade[e]);

  return 0;


}

int main(int argc, char *argv[]){
  int profundidade_container, linhas, colunas;
  char * fileName;
  int * matriz;

  int qtdIlhas;

  if(argc != 5){
    printf("Erro!, insira 5 parametros para funcionar:\n./EP1 <profundidade_container> <linhas_da_matriz> <colunas_da_matriz> exemplo.txt ");
    return -1;
  }else{
    profundidade_container = stringToInt(argv[1]);
    linhas = stringToInt(argv[2]);
    colunas= stringToInt(argv[3]);
    fileName = argv[4];

    printf("PROFUNDIDADE: %d\n", profundidade_container);
    printf("LINHAS: %d\n", linhas);
    printf("COLUNAS: %d\n", colunas);
    printf("EXEMPLO: %s\n", fileName);

    matriz =  montarMatriz(fileName, linhas, colunas);
    buscarIlhas(matriz, linhas, colunas);

    // for (int i = 0; i < linhas*colunas; i++)
    // {
    //   if(i&&!(i%colunas) )printf("\n");
    //   printf("%d ", matriz[i]);


    // }

    
    

  }
  

  scanf("%c");

  return 0;
}
